#include <pbc/pbc.h>
#include <pbc/pbc_test.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv) // argv[1] = string of k; argv[2] = string of size_of_operation
{
    char message1[1000];
    char s[3000] = "type a\nq 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\nh 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\nr 730750818665451621361119245571504901405976559617\nexp2 159\nexp1 107\nsign1 1\nsign0 1\n";
    
    
    int n = 100000;
    int k = atoi(argv[1]); // size_of_operation = 5, k ={ 10, 20, 40, 80, 160, 320, 640, 1280 }
    int size_of_operation = atoi(argv[2]); // size_of_operation < k; k = 160, size_of_operation = 5, 10, 20, 40, 80, 160
    
    
    //@Gwangwoon: k = 1000, size_of_operation = 100 --> 4 hours
    // Taeho guess @workstation: k = 100, size_of_operation = 1000 -> 1 hour
    
    // need to vary k & size_of_operation above.
    
    int size_of_pp = ((2 * n));
    int t, i;
    int size_of_j = k;
    int j = k;
    //n=<k
    //----------------------------------------------------------------
    
//    printf("Started \n");
    

    pairing_t pairing;
    double  time1, time2, time3, time4, time5, time6,
            time7,time8, time9, time10, time11, time12,
            time13, time14, time15, time16;
 

    
    element_t g;
    element_t x;
    element_t td; // = x
    //element_t I[(size_of_pp + 1)]; // set for {1,2,...,2n}
    element_t* I;
    I = (element_t*)malloc(sizeof(element_t) * (size_of_pp + 1));
    
    
    //element_t xi[(size_of_pp + 1)]; // x^1, ...,x^2n
    element_t* xi; // x^1, ...,x^2n
    xi = (element_t*)malloc(sizeof(element_t) * (size_of_pp + 1));
    
    //element_t pp[(size_of_pp + 1)]; //g^(x^i)
    element_t* pp; //g^(x^i)
    pp = (element_t*)malloc(sizeof(element_t) * (size_of_pp + 1));
    
    //element_t mi[k + 1]; // m1,m2...,mi
    element_t* mi;
    mi = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t mixi[k + 1]; // mi*x^i
    element_t* mixi;
    mixi = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t gmixi[k + 1]; // g^mi*x^i
    element_t* gmixi;
    gmixi = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    element_t r;  //randomly choose r from z_p
    element_t gr; //g^r
    element_t productgmixi;
    element_t D;
    //element_t n1j[(k + 1)];
    element_t* n1j;
    n1j = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t xn1j[(k + 1)];
    element_t* xn1j;
    xn1j = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    
    //element_t in1j[k + 1][(k + 1)];
    
    printf("Before 2D arrays \n");
    fflush(stdout);
    
    element_t** in1j;
    in1j = (element_t**)malloc(sizeof(element_t*) * (k + 1));
    for(i = 0; i < k + 1; i++){
        in1j[i] = (element_t*)malloc(sizeof(element_t) * ((k + 1)));
    }
    
    //element_t xin1j[k + 1][(k + 1)];
    element_t** xin1j;
    xin1j = (element_t**)malloc(sizeof(element_t*) * (k + 1));
    for(i = 0; i < k + 1; i++){
        xin1j[i] = (element_t*)malloc(sizeof(element_t) * ((k + 1)));
    }
    
    
    //element_t gmi[k + 1];
    element_t* gmi;
    gmi = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t gmixin1j[k + 1][(k + 1)];
    element_t** gmixin1j;
    gmixin1j = (element_t**)malloc(sizeof(element_t*) * (k + 1));
    for(i = 0; i < k + 1; i++){
        gmixin1j[i] = (element_t*)malloc(sizeof(element_t) * ((k + 1)));
    }
    
    //element_t grxn1j[(k + 1)];
    element_t* grxn1j;
    grxn1j = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    element_t tempxx;
    element_t tempx;
    //element_t ljleft[(k + 1)];
    element_t* ljleft;
    ljleft = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t lj[(k + 1)];
    element_t* lj;
    lj = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    element_t gxmj;
    element_t temp1;
    element_t temp2;
    element_t temp3;
    element_t temp4;
    //element_t newdataT[(size_of_operation + 1)];
    element_t* newdataT;
    newdataT = (element_t*)malloc(sizeof(element_t) * (size_of_operation + 1));
    
    //element_t timi[(size_of_operation + 1)];
    element_t* timi;
    timi = (element_t*)malloc(sizeof(element_t) * (size_of_operation + 1));
    
    //element_t gtimi[(size_of_pp + 1)];
    element_t* gtimi;
    gtimi = (element_t*)malloc(sizeof(element_t) * (size_of_pp + 1));
    
    //element_t gtimixi[(size_of_operation + 1)];
    element_t* gtimixi;
    gtimixi = (element_t*)malloc(sizeof(element_t) * (size_of_operation + 1));
    
    element_t tempxxx;
    element_t tempxxxx;
    element_t tempxxxxx;
    element_t tempxxxxxx;
    element_t productgtimixi;
    element_t newr;
    element_t gnewr;
    element_t newD1;
    element_t newD;
    //element_t newS[k + 1];
    element_t* newS;
    newS = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t ljhash[(k + 1)];
    element_t* ljhash;
    ljhash = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t Lj[(k + 1)];
    element_t* Lj;
    Lj = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    element_t productLj;
    //element_t ghj[(k + 1)];
    element_t* ghj;
    ghj = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    
    
    //element_t ghjxn1j[(k + 1)];
    element_t* ghjxn1j;
    ghjxn1j = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    
    element_t productghjxn1j;
    //element_t gmjhj[(k + 1)];
    element_t* gmjhj;
    gmjhj = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    element_t productgmjhj;
    element_t lastpairing;
    element_t temp5;
    element_t temp6;
    element_t temp7;
    element_t temp8;
    //element_t gnewmi[(k + 1)];
    element_t* gnewmi;
    gnewmi = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t gnewmixin1j[(k + 1)][(k + 1)];
    element_t** gnewmixin1j;
    gnewmixin1j = (element_t**)malloc(sizeof(element_t*) * (k + 1));
    for(i = 0; i < (k + 1); i++){
        gnewmixin1j[i] = (element_t*)malloc(sizeof(element_t) * ((k + 1)));
    }
    
    
    element_t tempk;
    //element_t pi[(k + 1)];
    element_t* pi;
    pi = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    //element_t equalprove[(k + 1)];
    element_t* equalprove;
    equalprove = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    element_t DdivnewD;
    element_t temp9;
    element_t temp10;
    //element_t gnewrxn1j[(k + 1)];
    element_t* gnewrxn1j;
    gnewrxn1j = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    
    //element_t pi1[(k + 1)];
    element_t* pi1;
    pi1 = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    
    element_t Dstar;
    element_t tempkk;
    //element_t gnewmixi[(k + 1)];
    element_t* gnewmixi;
    gnewmixi = (element_t*)malloc(sizeof(element_t) * (k + 1));
    
    element_t productgnewmixi;

    printf("Finished malloc \n");
    fflush(stdout);
    
    
    
    
    /*
    
    
    DECLARATION IS OVER
    
    
    
    */
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //----------------------------------------------------------------
    
    if (pairing_init_set_buf(pairing,s,strlen(s))) pbc_die("Pairing initialization failed.");
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");
    
    element_init_G1(g, pairing);
    element_init_Zr(x, pairing);
    element_init_Zr(td, pairing);
    element_init_Zr(r, pairing);
    element_init_G1(gr, pairing);
    element_init_G1(D, pairing);
    element_init_G1(tempxx, pairing);
    element_init_G1(gxmj,pairing);
    element_init_GT(temp1,pairing);
    element_init_GT(temp2,pairing);
    element_init_GT(temp3,pairing);
    element_init_GT(temp4,pairing);
    element_init_G1(tempx,pairing);
    element_init_G1(productgmixi,pairing);
    element_init_G1(tempxxx,pairing);
    element_init_G1(tempxxxx,pairing);
    element_init_G1(tempxxxxx,pairing);
    element_init_G1(tempxxxxxx,pairing);
    element_init_G1(productgtimixi,pairing);
    element_init_Zr(newr,pairing);
    element_init_G1(gnewr,pairing);
    element_init_G1(newD,pairing);
    element_init_G1(newD1,pairing);
    element_init_G1(productLj,pairing);
    element_init_G1(productghjxn1j,pairing);
    element_init_G1(productgmjhj,pairing);
    element_init_G1(lastpairing,pairing);
    element_init_GT(temp5,pairing);
    element_init_GT(temp6,pairing);
    element_init_GT(temp7,pairing);
    element_init_GT(temp8,pairing);
    element_init_G1(tempk,pairing);
    element_init_G1(DdivnewD,pairing);
    element_init_GT(temp9,pairing);
    element_init_GT(temp10,pairing);
    element_init_G1(tempkk,pairing);
    element_init_G1(productgnewmixi,pairing);
    element_init_G1(Dstar,pairing);

    
    //fflush(stdout);
    
    for(i = 0; i < (size_of_pp + 1); i++){
        element_init_Zr(I[i], pairing);
        
    }
    for(i = 0; i < (size_of_pp + 1); i++){
        element_init_Zr(xi[i], pairing);
        
    }
    for(i = 0; i < (size_of_pp + 1); i++){
        element_init_G1(pp[i], pairing);
        
    }
    for(i = 1; i < k + 1; i++){
        element_init_Zr(mi[i], pairing);}
    for(i = 1; i < k + 1; i++){
        element_init_Zr(mixi[i], pairing);
        
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(gmixi[i], pairing);
        
    }
    for(i = 1; i < k + 1; i++){
        element_init_Zr(n1j[i], pairing);}
    for(i = 1; i < k + 1; i++){
        for(t = 1; t < k + 1; t++){
            element_init_Zr(in1j[i][t],pairing);
        }
        
    }
    for(i = 1; i < k + 1; i++){
        for(t = 1; t < k + 1; t++){
            element_init_Zr(xin1j[i][t],pairing);
        }
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(gmi[i],pairing);
        
    }
    for(i = 1; i < k + 1; i++){
        for(t = 1; t < k + 1; t++){
            element_init_G1(gmixin1j[i][t],pairing);
        }
    }
    for(i = 1; i < k + 1; i++){
        element_init_Zr(xn1j[i], pairing);
        
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(grxn1j[i], pairing);
        
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(ljleft[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(lj[i],pairing);
    }
    for(i = 1; i < size_of_operation + 1; i++){
        element_init_Zr(newdataT[i],pairing);
    }
    for(i = 1; i < size_of_operation + 1; i++){
        element_init_Zr(timi[i],pairing);
    }
    for(i = 1; i < size_of_operation + 1; i++){
        element_init_G1(gtimi[i],pairing);
    }
    for(i = 1; i < size_of_operation + 1; i++){
        element_init_G1(gtimixi[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_Zr(newS[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_Zr(ljhash[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(Lj[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(ghj[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(ghjxn1j[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(gmjhj[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(gnewmi[i],pairing);
    }
    
    for(i = 1; i < k + 1; i++){
        for(t = 1; t < k + 1; t++){
            element_init_G1(gnewmixin1j[i][t],pairing);
        }}
    
    for(i = 1; i < k + 1; i++){
        element_init_G1(pi[i],pairing);
    }
    
    for(i = 1; i < k + 1; i++){
        element_init_G1(equalprove[i],pairing);
    }
    
    for(i = 1; i < k + 1; i++){
        element_init_G1(gnewrxn1j[i],pairing);
    }
    for(i = 1; i < k + 1; i++){
        element_init_G1(pi1[i],pairing);
    }
   
    for(i = 1; i < k + 1; i++){
        element_init_G1(gnewmixi[i],pairing);
        
    }
    
    
    
    
    /*
    
    
    
    INITIALIZATION IS OVER
    
    
    
    
    */
    
    
    
    
    printf("Init finished\n");
    
    
    
    
    
    
    //----------------------------------------------------------------

    // randomly choose an element g from G
    element_random(g);
//    element_printf("g = %B\n", g);

    //randomly choose an element x from Zp
    element_random(x);
//    element_printf("x = %B\n", x);

    // set td=x
    element_set(td,x);
//    element_printf("td = %B\n", td);
    
    for(i = 0; i < (size_of_pp + 1); i++){
        element_set_si(I[i], i);
//        element_printf("I[%d] is %B\n", i, I[i]);
    }
        
    //set for x^i
    for(i = 0; i < (size_of_pp + 1); i++){
        element_pow_zn(xi[i],x, I[i]);
//        element_printf("x^%d is %B\n", i, xi[i]);
    }
    
    //set for Gi= g^(x^i)
    for(i = 0; i < (size_of_pp + 1); i++){
        element_pow_zn(pp[i],g, xi[i]);
//        element_printf("pp[%d] = G_%d = g^(x^%d) is %B\n", i,i,i, pp[i]);
    }
    
    //----------------------------------------------------------------

    //randomly choose r from z_p
    element_random(r);
//    element_printf("r = %B\n", r);
    
    // set for m1,m2...,mi comming from Zp randomly
    for(i = 1; i < k + 1; i++){
        element_random(mi[i]);
//        element_printf("m%d = %B\n",i, mi[i]);
    }
    
    time1 = pbc_get_time(); // start of commitment
    
    // set for m1x^1,m2x^2...,mix^i
    for(i = 1; i < k + 1; i++){
        element_mul(mixi[i],mi[i],xi[i]);
//        element_printf("m%d * x^%d = %B\n",i, i,mixi[i]);
    }
    
    //set for g^(m1x^1,m2x^2...,mix^i)
    for(i = 1; i < k + 1; i++){
        element_pow_zn(gmixi[i], g,mixi[i]);
//        element_printf("g^(m%dx^%d) = %B\n",i, i,gmixi[i]);
    }
    
    //gr=g^r
    element_pow_zn(gr, g, r);
//    element_printf("g^r = %B\n", gr);
    

    element_set1(tempx); // let tempxx = 1

    for(i = 1; i < k + 1; i++){
        element_mul(tempx, tempx, gmixi[i]);
        element_set(productgmixi,tempx);
        }
//    element_printf("test=%B\n",productgmixi);
    
    //D
    element_mul(D,productgmixi,gr);
//    element_printf("D = %B\n",D);
    
    time2 = pbc_get_time(); // end of commitment
    //----------------------------------------------------------------
        
    printf("finished commitment\n");
    
    time3 = pbc_get_time(); // start of member proof generation

    for(i = 1; i < k + 1; i++){
    element_set_si(n1j[i], n + 1 - i);
//    element_printf("n+ 1-j = %d+ 1-%d = %B\n", n,i,n1j[i]);
    }
        
    for(i = 1; i < k + 1; i++){
        for(t = 1; t < k + 1; t++){
            element_set_si(in1j[i][t], i + n + 1 - t);
//            element_printf("i+n+ 1-j = %d+%d+ 1-%d = %B\n", i,n,t,in1j[i][t]);
        }
    }
    for(i = 1; i < k + 1; i++){
        for(t = 1; t < k + 1; t++){
            element_pow_zn(xin1j[i][t], x, in1j[i][t]);
//            element_printf("x^(i+n+ 1-j) = x^%B = %B\n", in1j[i][t],xin1j[i][t]);
        }
    }
    
    for(i = 1; i < k + 1; i++){
        element_pow_zn(gmi[i], g, mi[i]);
//        element_printf("g^m%d=%B\n",i,gmi[i]);
    }
    
    for(i = 1; i < k + 1; i++){
        for(t = 1; t < k + 1; t++){
            element_pow_zn(gmixin1j[i][t], gmi[i], xin1j[i][t]);
//            element_printf("g^(m%d * x^%B) = %B\n", i, in1j[i][t],gmixin1j[i][t]);
        }
        
    }
    
    
    for(i = 1; i < k + 1; i++){
        element_pow_zn(xn1j[i], x, n1j[i]);
    }
    
    for(i = 1; i < k + 1; i++){
    element_pow_zn(grxn1j[i],gr,xn1j[i]);
//        element_printf("Test=%B\n",grxn1j[i]);
    }

    for(i = 1; i < k + 1; i++){
        element_set1(tempxx); // let tempxx = 1
        
        for(int x = 1; x < k + 1; x++){
            if (x == i) continue;
            //            element_mul(ljleft[j], tempxx, gmixin1j[x][j]);
            element_mul(tempxx, tempxx, gmixin1j[x][i]);
            element_set(ljleft[i],tempxx);
        }}
//    element_printf("ljleft[%d]=%B\n",1,ljleft[1]);

  
    for(i = 1; i < k + 1; i++){
        element_mul(lj[i],ljleft[i],grxn1j[i]);
//        element_printf("lj[%d]=%B\n",i,lj[i]);
    }
    
    

    element_pow_zn(gxmj,pp[1],mi[j]);
    
    time4 = pbc_get_time(); // end of member proof generation
    
    printf("finished member proof gen\n");
    
    time5 = pbc_get_time(); // start of member verify
    
    element_pairing(temp1, D, pp[n + 1 - j]);
    element_pairing(temp2, lj[j], pp[0]);
    element_pairing(temp3, gxmj, pp[n]);
    element_mul(temp4,temp2,temp3);
    if (!element_cmp(temp1, temp4)) printf("1\n");
    else printf("0\n");
    time6 = pbc_get_time(); // end of member verify

    //----------------------------------------------------------------
    

    for(i = 1; i < size_of_operation+ 1; i++){
        element_random(newdataT[i]);
//        element_printf("s%d = %B\n",i, newdataT[i]);
    }
    
    time7 = pbc_get_time(); // start of update commitment

    for(i = 1; i < size_of_operation+ 1; i++){
        element_sub(timi[i],newdataT[i],mi[i]);
        //element_printf("t%d-m%d = %B\n",i,i, timi[i]);
    }
    for(i = 1; i < size_of_operation+ 1; i++){
        element_pow_zn(gtimi[i],g,timi[i]);
//        element_printf("g^t%d-m%d = %B\n",i,i, gtimi[i]);
    }
    
    for(i = 1; i < size_of_operation+ 1; i++){
        element_pow_zn(gtimixi[i],gtimi[i],xi[i]);
      // element_printf("g^((t%d-m%d)*x^%d) = %B\n",i,i,i, gtimixi[i]);
        }
    
    element_set1(tempxxx); // let tempxxx = 1

    for(i = 1; i <size_of_operation+ 1; i++){
        element_mul(tempxxx, tempxxx, gtimixi[i]);
        element_set(productgtimixi,tempxxx);
        }
    // element_printf("test = %B\n", productgtimixi);
    
    element_random(newr);
    element_pow_zn(gnewr,g,newr);
    
    element_mul(newD1,productgtimixi,gnewr);
    
    element_div(newD,newD1,gr);
    
    time8 = pbc_get_time(); // end of update commitment
    
    printf("finished update commit\n");
    
    for(i = 1; i < size_of_operation+ 1; i++){
        element_set(newS[i],newdataT[i]);
    }
    for(i = size_of_operation+ 1; i < k + 1; i++){
        element_set(newS[i],mi[i]);
    }

    //----------------------------------------------------------------

    //have to check//
    //have to check//
    //have to check//
    //have to check//
    //have to check//

    
    time9 = pbc_get_time(); // start of aggregate proof generation

    for(i = 1; i < size_of_j + 1; i++){
    element_set_si(ljhash[i],i);
//        element_printf("test= %B\n",ljhash[i]);
    }
    
    for(i = 1; i < size_of_j + 1; i++){
    element_from_hash(ljhash[i], "hash", i);
//        element_printf("test=%B\n",ljhash[i]);
    }
    
    //have to check//
    //have to check//
    //have to check//
    //have to check//
    //have to check//
    
    
    
    for(i = 1; i < size_of_j + 1; i++){
        element_pow_zn(Lj[i],lj[i],ljhash[i]);
    }

    element_set1(tempxxxx); // let tempxxx = 1

    for(i = 1; i < size_of_j + 1; i++){
        element_mul(tempxxxx, tempxxxx, Lj[i]);
        
    }
    element_set(productLj,tempxxxx);
    time10 = pbc_get_time(); // end of aggregate proof generation

    time11 = pbc_get_time(); // start of aggregate verify

    for(i = 1; i < size_of_j + 1; i++){
        element_pow_zn(ghj[i], g, ljhash[i]);
        }
    
    for(i = 1; i < size_of_j + 1; i++){
        element_pow_zn(ghjxn1j[i], ghj[i], xn1j[i]);
        }
    
    element_set1(tempxxxxx); // let tempxxx = 1
    for(i = 1; i < size_of_j + 1; i++){
        element_mul(tempxxxxx, tempxxxxx, ghjxn1j[i]);
        
    }
    element_set(productghjxn1j,tempxxxxx);
    
    for(i = 1; i < size_of_j + 1; i++){
        element_pow_zn(gmjhj[i],ghj[i],mi[i]);
    }
    element_set1(tempxxxxxx); // let tempxxx = 1
    for(i = 1; i < size_of_j + 1; i++){
        element_mul(tempxxxxxx, tempxxxxxx, gmjhj[i]);
        
    }
    element_set(productgmjhj,tempxxxxxx);

    
    
    element_pow_zn(lastpairing,productgmjhj,x);
    
    element_pairing(temp5,D, productghjxn1j);
    element_pairing(temp6,productLj, pp[0]);
    element_pairing(temp7,lastpairing ,pp[n]);
    element_mul(temp8,temp6,temp7);
    if (!element_cmp(temp5, temp8)) printf("1\n");
    else printf("0\n");
    time12 = pbc_get_time(); // end of aggregate verify
    
    
    printf("finished agg verify\n");
    
    

    //----------------------------------------------------------------
    
    time13 = pbc_get_time(); // start of equal proof generation
    for(i = 1; i < k + 1; i++){
        element_pow_zn(gnewmi[i],g,newS[i]);
    }
    for(i = 1; i < k + 1; i++){
        for(t = 1; t < k + 1; t++){
            element_pow_zn(gnewmixin1j[i][t],gnewmi[i], xin1j[i][t]);
        }
    }
    for(i = 1; i < k + 1; i++){
        element_set1(tempk); // let tempk = 1
        
        for(int w = 1; w <  k + 1; w++){
            if (w == i) continue;
            element_mul(tempk, tempk, gnewmixin1j[w][i]);
            
       }
        element_set(pi[i],tempk);
        
    }
    
    for(i = 1; i < k + 1; i++){
        element_pow_zn(gnewrxn1j[i],gnewr,xn1j[i]);
    }
    
    for(i = 1; i < k + 1; i++){
        element_mul(pi1[i],pi[i],gnewrxn1j[i]);
    }
    
    for(i = 1; i < k + 1; i++){
        element_div(equalprove[i],lj[i],pi1[i]);
    }
    time14 = pbc_get_time(); // end of equal proof generation

    printf("finished equal proof gen\n");
    
    
    //----------------------------------------------------------------
    
    for(i = 1; i < k + 1; i++){
        element_pow_zn(gnewmixi[i],gnewmi[i],xi[i]);
    }
    
    element_set1(tempkk); // let tempxx = 1

    for(i = 1; i <k + 1; i++){
        element_mul(tempkk, tempkk, gnewmixi[i]);
        
    }
    element_set(productgnewmixi,tempkk);
    
    element_mul(Dstar,productgnewmixi,gnewr);
    

    element_div(DdivnewD,D,Dstar);
    
    time15 = pbc_get_time(); // start of equal verify

    
    element_pairing(temp9,DdivnewD, pp[n+ 1 - j]);
    element_pairing(temp10,equalprove[j], pp[0]);
    
    if (!element_cmp(temp9, temp10)) printf("1\n");
    else printf("0\n");
     
    
    time16 = pbc_get_time(); // end of equal verify

    
    printf("%d,%d,%d,%fs,%fs,%fs,%fs,%fs,%fs,%fs,%fs", n, k, size_of_operation, time2 - time1, time4 - time3, time6 - time5, time8 - time7, time10 - time9, time12 - time11, (time14 - time13), k * (time16 - time15));
    
    
//    printf("CommitSet = %fs\n", time2 - time1); // time of commitment set generation
//    printf("MemberProve = %fs\n", time4 - time3); // time of member proof generation
//    printf("MemberVerify = %fs\n", time6 - time5); // time of member verification
//    printf("MUpdateCommitmnet = %fs\n", time8 - time7); // time of commitment update
//    printf("AggProof = %fs\n", time10 - time9); // time of agg proof generation
//    printf("AggVerify = %fs\n", time12 - time11); // time of agg verification
//    printf("EqualProve = %fs\n", (time14 - time13) * k); // time of equal proof generation
//    printf("EqualVerify = %fs\n", (time16 - time15) * k); // time of equal verify

    return(0);
}


