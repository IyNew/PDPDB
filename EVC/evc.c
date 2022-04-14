#include <pbc/pbc.h>
#include <pbc/pbc_test.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define N 100000

void CommitSet(pairing_t pairing, element_t com, element_t *pp, element_t *mi, element_t s, int k, bool b)
{
    double tic, toc;
    int i;

    element_t *tmp;
    tmp = (element_t *)malloc(sizeof(element_t) * (k + 1));

    for (i = 0; i < k + 1; i++)
    {
        element_init_G1(tmp[i], pairing);
    }

    // declaration && initialization is done

    tic = pbc_get_time(); // start of commitment

    // g^s
    element_pow_zn(tmp[0], pp[0], s);

    // set for g^(m1x^1,m2x^2...,mix^i)
    for (i = 1; i < k + 1; i++)
    {
        element_pow_zn(tmp[i], pp[i], mi[i]);
    }

    for (i = 1; i < k + 1; i++)
    {
        element_mul(tmp[i], tmp[i], tmp[i - 1]);
    }

    toc = pbc_get_time(); // end of commitment

    element_set(com, tmp[k]); // commitment

    if (b == 1)
    {
        printf("%.10lf,", (double)toc - (double)tic); // This is the CommitSet Runtime
    }
    free(tmp);
}

void MemberProve(pairing_t pairing, element_t lj, element_t *pp, element_t *mi, int k, int j, element_t r, bool b) // proof for j-th position l_j
{
    // pairing_t pairing;
    double tic, toc;
    int i, n = N;

    element_t tempx;
    element_t *tmp;
    tmp = (element_t *)malloc(sizeof(element_t) * (k + 1));

    for (i = 0; i < k + 1; i++)
    {
        element_init_G1(tmp[i], pairing);
    }

    element_set1(tmp[j]); // let tmp[j] = 1

    // declaration && initialization is done

    tic = pbc_get_time(); // start of member proof generation

    element_pow_zn(tmp[0], pp[n + 1 - j], r);

    for (i = 1; i < k + 1; i++)
    {
        if (i == j)
            continue;
        element_pow_zn(tmp[i], pp[i + n + 1 - j], mi[i]);
    }

    for (i = 1; i < k + 1; i++)
    {
        element_mul(tmp[i], tmp[i], tmp[i - 1]);
    }

    element_set(lj, tmp[k]);

    toc = pbc_get_time();

    if (b == 1)
    {
        printf("%.10lf,", (double)toc - (double)tic); // This is the MemberProve Runtime
    }
    free(tmp);
}
void MemberVerify(pairing_t pairing, element_t *pp, int j, int k, element_t lj, element_t D, element_t m)
{
    double tic, toc;
    int i, n = N;

    element_t G1mj, tmp1, tmp2, tmp3, tmp4;
    // element_t* tmp;
    // tmp = (element_t*)malloc(sizeof(element_t) * (k + 1));

    // for(i = 0; i < k + 1; i++){
    //     element_init_G1(tmp[i], pairing);
    // }

    element_init_G1(G1mj, pairing);
    element_init_GT(tmp1, pairing);
    element_init_GT(tmp2, pairing);
    element_init_GT(tmp3, pairing);
    element_init_GT(tmp4, pairing);

    // declaration && initialization is done

    tic = pbc_get_time(); // start of member verification

    element_pow_zn(G1mj, pp[1], m);

    element_pairing(tmp1, D, pp[n + 1 - j]);
    element_pairing(tmp2, lj, pp[0]);
    element_pairing(tmp3, G1mj, pp[n]);
    element_mul(tmp4, tmp2, tmp3);

    toc = pbc_get_time();

    // if (!element_cmp(tmp1, tmp4)) printf("pass the verification\n");
    // else printf("fail to verify\n");

    printf("%.10lf,", (double)toc - (double)tic); // This is the MemberVerify runtime
}

void AggProof(pairing_t pairing, element_t Lj, element_t *pp, element_t D, element_t *mi, element_t *lj, int num_agg)
{

    double tic, toc;
    int i, l = num_agg;

    element_t tempx;
    element_t *tmp, *hash;
    tmp = (element_t *)malloc(sizeof(element_t) * (l + 1));
    hash = (element_t *)malloc(sizeof(element_t) * (l + 1));

    for (i = 0; i < l + 1; i++)
    {
        element_init_G1(tmp[i], pairing);
        element_init_G1(hash[i], pairing);
    }

    element_init_G1(Lj, pairing);
    element_init_G1(tempx, pairing);

    element_set1(tempx); // let tmp[j] = 1

    for (i = 1; i < l + 1; i++)
    {
        element_set_si(hash[i], i);
    }

    for (i = 1; i < l + 1; i++)
    {
        element_from_hash(hash[i], "hash", i);
    }

    // have to check//
    // have to check//
    // have to check//
    // have to check//
    // have to check//

    // declaration && initialization is done

    tic = pbc_get_time(); // start

    for (i = 1; i < l + 1; i++)
    {
        element_pow_zn(tmp[i], lj[i], hash[i]);
    }

    for (i = 1; i < l + 1; i++)
    {
        element_mul(tempx, tempx, tmp[i]);
    }
    element_set(Lj, tempx);

    toc = pbc_get_time(); // end

    printf("%.10lf,", (double)toc - (double)tic); // This is the AggProof runtime

    free(tmp);
    free(hash);
}

void AggVerify(pairing_t pairing, element_t *pp, element_t Lj, element_t D, element_t *mi, int num_agg)
{

    double tic, toc;
    int i, l = num_agg, n = N;

    element_t tempx, LHSr, RHS1, RHS2, RHS, LHS;
    element_t *tmp, *hash;
    tmp = (element_t *)malloc(sizeof(element_t) * (l + 1));
    hash = (element_t *)malloc(sizeof(element_t) * (l + 1));

    for (i = 0; i < l + 1; i++)
    {
        element_init_G1(tmp[i], pairing);
        element_init_G1(hash[i], pairing);
    }

    element_init_G1(Lj, pairing);
    element_init_G1(LHSr, pairing);
    element_init_GT(LHS, pairing);
    element_init_GT(RHS1, pairing);
    element_init_GT(RHS2, pairing);
    element_init_GT(RHS, pairing);
    element_init_G1(tempx, pairing);

    element_set1(tempx); // let tmp[j] = 1

    for (i = 1; i < l + 1; i++)
    {
        element_set_si(hash[i], i);
    }

    for (i = 1; i < l + 1; i++)
    {
        element_from_hash(hash[i], "hash", i);
    }

    // declaration && initialization is done

    tic = pbc_get_time(); // start of aggregate verify

    for (i = 1; i < l + 1; i++)
    {
        element_pow_zn(tmp[i], pp[n + 1 - i], hash[i]);
    }

    for (i = 1; i < l + 1; i++)
    {
        element_mul(tempx, tempx, tmp[i]);
    }
    element_set(LHSr, tempx);

    for (i = 1; i < l + 1; i++)
    {
        element_mul(hash[i], mi[i], hash[i]);
    }

    for (i = 1; i < l + 1; i++)
    {
        element_pow_zn(tmp[i], pp[1], hash[i]);
    }
    element_set1(tempx); // let tempx = 1
    for (i = 1; i < l + 1; i++)
    {
        element_mul(tempx, tempx, tmp[i]);
    }

    // element_pow_zn(lastpairing,productgmjhj,x);

    element_pairing(LHS, D, LHSr);
    element_pairing(RHS1, Lj, pp[0]);
    element_pairing(RHS2, tempx, pp[n]);
    element_mul(RHS, RHS1, RHS2);

    // if (!element_cmp(LHS, RHS)) printf("1\n");
    // else printf("0\n");
    toc = pbc_get_time(); // end of aggregate verify

    printf("%.10lf,", (double)toc - (double)tic); // This is the AggVerify runtime

    free(tmp);
    free(hash);
}
void EqualProve(pairing_t pairing, element_t *pp, element_t eq_proof, element_t *mi, element_t *newmi, element_t r, element_t newr, int eq_idx, int num_mi)
{
    double tic, toc;
    int i, l, n = N;

    l = num_mi; // assume that: the index set of mi's = the index set of newmi's = {1,...,l}

    element_t tempx;
    element_t *tmp, *tmp_z;
    tmp = (element_t *)malloc(sizeof(element_t) * (l + 1));
    tmp_z = (element_t *)malloc(sizeof(element_t) * (l + 1));

    for (i = 0; i < l + 1; i++)
    {
        element_init_G1(tmp[i], pairing);
        element_init_Zr(tmp_z[i], pairing);
    }

    element_init_G1(tempx, pairing);

    element_set1(tempx); // let tmp[j] = 1

    tic = pbc_get_time(); // start

    element_sub(tmp_z[0], r, newr);

    // in fact, it suffices to calculate the sub. only with the updated components
    for (i = 1; i < l + 1; i++)
    {
        element_sub(tmp_z[i], mi[i], newmi[i]);
    }

    element_set1(tmp[eq_idx]); // let tmp[eq_idx] = 1

    // declaration && initialization is done

    for (i = 0; i < l + 1; i++)
    {
        if (i == eq_idx)
            continue;
        element_pow_zn(tmp[i], pp[i + n + 1 - eq_idx], tmp_z[i]);
    }

    for (i = 1; i < l + 1; i++)
    {
        element_mul(tmp[i], tmp[i], tmp[i - 1]);
    }

    element_set(eq_proof, tmp[l]);

    toc = pbc_get_time(); // end of equal proof generation

    printf("%.10lf,", (double)toc - (double)tic); // This is the EqualProve runtime

    free(tmp);
    free(tmp_z);
}

void EqualVerify(pairing_t pairing, element_t *pp, element_t D, element_t newD, element_t eq_proof, int eq_idx)
{
    double tic, toc;
    int i, l, n = N;

    element_t tempx;
    element_t tmp1, tmp2;

    element_init_G1(tempx, pairing);
    element_init_GT(tmp1, pairing);
    element_init_GT(tmp2, pairing);

    element_set1(tempx); // let tempx = 1

    tic = pbc_get_time(); // start of equal verify

    element_div(tempx, D, newD);
    element_pairing(tmp1, tempx, pp[n + 1 - eq_idx]);
    element_pairing(tmp2, eq_proof, pp[0]);

    // if (!element_cmp(tmp1, tmp2)) printf("1\n");
    // else printf("0\n");

    toc = pbc_get_time(); // end of equal verification

    printf("%.10lf\n", (double)toc - (double)tic); // This is the EqualVerify Runtime, which is the last one so I'm adding \n at the end.
}

int main(int argc, char **argv) // argv[1] = string of k; argv[2] = string of size_T
{
    char message1[1000];
    char s[3000] = "type a\nq 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\nh 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\nr 730750818665451621361119245571504901405976559617\nexp2 159\nexp1 107\nsign1 1\nsign0 1\n";

    int n = N;
    int k = atoi(argv[1]);      // size_of_operation = 5, k ={ 10, 20, 40, 80, 160, 320, 640, 1280 }
    int size_T = atoi(argv[2]); // size_of_operation < k; k = 160, size_of_operation = 5, 10, 20, 40, 80, 160

    int size_of_pp = ((2 * n));
    int t, i;

    int j = k;

    // size_T cannot exceed k

    // n=<k
    //----------------------------------------------------------------

    printf("100000,%d,%d,", k, size_T);

    pairing_t pairing;

    element_t g;
    element_t x; // trapdoor
    element_t *I;
    I = (element_t *)malloc(sizeof(element_t) * (size_of_pp + 1));

    element_t *xi; // x^1, ...,x^2n
    xi = (element_t *)malloc(sizeof(element_t) * (size_of_pp + 1));

    element_t *pp; // g^(x^i)
    pp = (element_t *)malloc(sizeof(element_t) * (size_of_pp + 1));

    element_t *mi;
    mi = (element_t *)malloc(sizeof(element_t) * (k + 1));

    element_t r, newr, r_T; // randomly choose r from z_p, r_T is for contents commitment
    element_t gr;           // g^r
    element_t D;

    element_t tempx;

    element_t *lj;
    lj = (element_t *)malloc(sizeof(element_t) * (k + 1));

    element_t *newmi; // let newS = {(1,newmi[1]), ..., (k,newmi[k])}
    newmi = (element_t *)malloc(sizeof(element_t) * (k + 1));

    element_t newD;

    element_t *equalprove;
    equalprove = (element_t *)malloc(sizeof(element_t) * (k + 1));

    // printf("Finished malloc \n");

    /*

    DECLARATION IS OVER

    */

    //----------------------------------------------------------------

    if (pairing_init_set_buf(pairing, s, strlen(s)))
        pbc_die("Pairing initialization failed.");
    if (!pairing_is_symmetric(pairing))
        pbc_die("pairing must be symmetric");

    element_init_G1(g, pairing);
    element_init_Zr(x, pairing);
    element_init_Zr(r, pairing);
    element_init_Zr(r_T, pairing);
    element_init_G1(gr, pairing);
    element_init_G1(D, pairing);
    element_init_G1(tempx, pairing);
    element_init_Zr(newr, pairing);
    element_init_G1(newD, pairing);

    for (i = 0; i < (size_of_pp + 1); i++)
    {
        element_init_Zr(I[i], pairing);
    }
    for (i = 0; i < (size_of_pp + 1); i++)
    {
        element_init_Zr(xi[i], pairing);
    }

    for (i = 0; i < (size_of_pp + 1); i++)
    {
        element_init_G1(pp[i], pairing);
    }

    for (i = 1; i < k + 1; i++)
    {
        element_init_Zr(mi[i], pairing);
    }

    for (i = 1; i < k + 1; i++)
    {
        element_init_G1(lj[i], pairing);
    }

    for (i = 1; i < k + 1; i++)
    {
        element_init_Zr(newmi[i], pairing);
    }

    for (i = 1; i < k + 1; i++)
    {
        element_init_G1(equalprove[i], pairing);
    }

    /*

    INITIALIZATION IS OVER

    */

    // printf("Init finished\n");

    //----------------------------------------------------------------

    // randomly choose an element g from G
    element_random(g);

    // randomly choose an element x from Zp
    element_random(x);

    for (i = 0; i < (size_of_pp + 1); i++)
    {
        element_set_si(I[i], i);
    }

    // set for x^i
    for (i = 0; i < (size_of_pp + 1); i++)
    {
        element_pow_zn(xi[i], x, I[i]);
    }

    // set for Gi= g^(x^i)
    for (i = 0; i < (size_of_pp + 1); i++)
    {
        element_pow_zn(pp[i], g, xi[i]);
    }

    free(I);
    free(xi);

    //----------------------------------------------------------------

    // randomly choose r from z_p
    element_random(r);

    // set for m1,m2...,mi comming from Zp randomly
    for (i = 1; i < k + 1; i++)
    {
        element_random(mi[i]);
    }

    CommitSet(pairing, D, pp, mi, r, k, 1);

    //----------------------------------------------------------------

    // printf("finished commitment\n");

    MemberProve(pairing, lj[j], pp, mi, k, j, r, 1);

    // printf("finished member proof gen\n");

    MemberVerify(pairing, pp, j, k, lj[j], D, mi[j]);

    //----------------------------------------------------------------

    for (i = 1; i < k + 1; i++)
    {
        element_random(newmi[i]);
    }

    // randomly choose newr from z_p
    element_random(newr);
    element_random(r_T);

    element_t comT, com_tmp;
    int eq_idx = k; // It should be an index of equal posion

    element_t *tmp_z;
    tmp_z = (element_t *)malloc(sizeof(element_t) * (k + 1));

    for (i = size_T + 1; i < k + 1; i++)
    {
        element_set(newmi[i], mi[i]); // assume that newmi[i] = mi[i] except first size_T position
    }

    element_init_G1(comT, pairing);
    element_init_G1(com_tmp, pairing);

    CommitSet(pairing, com_tmp, pp, newmi, newr, size_T, 0); // temporary value for update
    CommitSet(pairing, comT, pp, newmi, r_T, size_T, 0);     // for simplicity, T={(1,newmi[1]),...,(size_T, newmi[size_T])}

    double tic, toc;
    element_t *tmp;
    tmp = (element_t *)malloc(sizeof(element_t) * (k + 1));

    for (i = 0; i < k + 1; i++)
    {
        element_init_G1(tmp[i], pairing);
    }

    // declaration && initialization is done

    tic = pbc_get_time(); // start of commitment

    element_pow_zn(tmp[0], pp[0], r);

    // for the index I_T
    for (i = 1; i < size_T + 1; i++)
    {
        element_pow_zn(tmp[i], pp[i], mi[i]);
    }

    for (i = 1; i < size_T + 1; i++)
    {
        element_mul(tmp[i], tmp[i], tmp[i - 1]);
    }

    // element_set(com_tmp, tmp[k]);//commitment for S[I_T] w/ random r

    element_mul(tempx, D, com_tmp);

    element_div(newD, tempx, tmp[size_T]);

    toc = pbc_get_time();

    // printf("UpdateCommit Runtime: \n");
    printf("%.10lf,", (double)toc - (double)tic); // This is the UpdateCommit runtime

    // printf("finished update commit\n");

    //----------------------------------------------------------------

    for (i = 1; i < size_T + 1; i++)
    {
        MemberProve(pairing, lj[i], pp, mi, k, i, r, 0);
    }

    element_t Lj;
    element_init_G1(Lj, pairing);

    AggProof(pairing, Lj, pp, D, mi, lj, size_T);

    AggVerify(pairing, pp, Lj, D, mi, size_T);

    // printf("finished agg verify\n");

    //————————————————————————————————

    for (i = 0; i < k + 1; i++)
    {
        element_init_Zr(tmp_z[i], pairing);
    }

    element_t eq_proof;

    element_init_G1(eq_proof, pairing);

    // equality poof gen for the position (eq_idx)\in[n]
    EqualProve(pairing, pp, eq_proof, mi, newmi, r, newr, eq_idx, k);

    // printf("finished equal proof gen\n");

    //————————————————————————————————

    EqualVerify(pairing, pp, D, newD, eq_proof, eq_idx);

    // printf("finished equal proof gen\n");

    free(pp);
    free(mi);
    free(lj);
    free(newmi);
    free(equalprove);

    return (0);
}
