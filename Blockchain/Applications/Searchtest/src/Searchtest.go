package main

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"math"
	"strings"
	"time"

	"github.com/hyperledger/fabric-chaincode-go/shim"
	"github.com/hyperledger/fabric-contract-api-go/contractapi"
)

var initNum int = 10000

// SmartContract provides functions for managing an Record
type SmartContract struct {
	contractapi.Contract
}

// Record describes the provenance node structure
type Record struct {
	ID       string `json:"ID"`
	Previous string `json:"Previous"`
	// Data     string `json:"Data"`
	Tag   string `json:"Tag"`
	DBCom string `json:"DBCom"`
	Logs  string `json:"Logs"`
}

// nodes in Que
type node struct {
	value interface{}
	prev  *node
	next  *node
}

// Que
type LinkedQueue struct {
	head *node
	tail *node
	size int
}

// Selector query string constructor
type Selector struct {
	Members []SelectorMember `json:"$or"`
}

// Selector query string constructor
type SelectorMember struct {
	ID string `json:"previous"`
}

func readRecordFile() string {
	// Read the file with json records
	b, err := ioutil.ReadFile("./SR.json")
	if err != nil {
		fmt.Print(err)
	}
	str := string(b)
	return str
}

func deserializeJson(recordJson string) []Record {
	jsonAsBytes := []byte(recordJson)
	records := make([]Record, 0)
	err := json.Unmarshal(jsonAsBytes, &records)
	// fmt.Printf("%#v", records)
	if err != nil {
		panic(err)
	}
	return records
}

func GetStringForSelctorMemberListFromString(future string) string {
	var memberList []SelectorMember
	strList := strings.Split(future, "|")
	if len(strList) == 0 {
		return ""
	}
	for i := 0; i < len(strList); i++ {
		// fmt.Println("i=", i, strList[i])
		if strList[i] != "" {
			var member SelectorMember
			member.ID = strList[i]
			memberList = append(memberList, member)
		}
	}
	selector := Selector{
		Members: memberList,
	}
	q, err := json.Marshal(selector)
	if err != nil {
	}
	finalQstring := `{"selector":` + string(q) + `}`

	return finalQstring
}

func Float32ToByte(float float32) []byte {
	bits := math.Float32bits(float)
	bytes := make([]byte, 4)
	binary.LittleEndian.PutUint32(bytes, bits)

	return bytes
}

func IntToBytes(n int) []byte {
	data := int64(n)
	bytebuf := bytes.NewBuffer([]byte{})
	binary.Write(bytebuf, binary.BigEndian, data)
	return bytebuf.Bytes()
}

// Get size
func (queue *LinkedQueue) Size() int {
	return queue.size
}

// Peek
func (queue *LinkedQueue) Peek() interface{} {
	if queue.head == nil {
		panic("Empty queue.")
	}
	return queue.head.value
}

// Add
func (queue *LinkedQueue) Add(value interface{}) {
	new_node := &node{value, queue.tail, nil}
	if queue.tail == nil {
		queue.head = new_node
		queue.tail = new_node
	} else {
		queue.tail.next = new_node
		queue.tail = new_node
	}
	queue.size++
	new_node = nil
}

// Remove
func (queue *LinkedQueue) Remove() {
	if queue.head == nil {
		panic("Empty queue.")
	}
	first_node := queue.head
	queue.head = first_node.next
	first_node.next = nil
	first_node.value = nil
	queue.size--
	first_node = nil
}

// InitLedger adds a base set of records to the ledger
func (s *SmartContract) InitLedger(ctx contractapi.TransactionContextInterface) error {
	// import records from a json file
	recordList := readRecordFile()
	unmarshalledRecords := deserializeJson(recordList)
	// define a counter to avoid transaction timeout
	var counter int = 0
	for _, record := range unmarshalledRecords {
		recordJSON, err := json.Marshal(record)
		if err != nil {
			return err
		}

		err = ctx.GetStub().PutState(record.ID, recordJSON)
		if err != nil {
			println(err)
		}
		// println(record.ID, " is done")
		counter++
		if counter > initNum {
			break
		}
		if err != nil {
			return fmt.Errorf("failed to put to world state. %v", err)
		}
	}

	return nil
}

// second init, keep putting records into the ledger if not finished in InitLedger
func (s *SmartContract) SecondInit(ctx contractapi.TransactionContextInterface, round int) error {

	recordList := readRecordFile()
	unmarshalledRecords := deserializeJson(recordList)
	println(initNum*(round), " records are added")

	var counter int = 0
	for _, record := range unmarshalledRecords {
		counter++
		if counter <= initNum*round {
			continue
		}
		if counter > initNum*(round+1) {
			break
		}
		recordJSON, err := json.Marshal(record)
		if err != nil {
			return err
		}

		err = ctx.GetStub().PutState(record.ID, recordJSON)
		if err != nil {
			println(err)
		}
		// println(record.ID, " is done")

		if err != nil {
			return fmt.Errorf("failed to put to world state. %v", err)
		}
	}

	return nil
}

// CreateRecord issues a new record to the world state with given details.
// func (s *SmartContract) CreateRecord(ctx contractapi.TransactionContextInterface, id string, previous string, data string) error {
// 	exists, err := s.RecordExists(ctx, id)
// 	if err != nil {
// 		return err
// 	}
// 	if exists {
// 		return fmt.Errorf("the record %s already exists", id)
// 	}

// 	record := Record{
// 		ID:       id,
// 		Previous: previous,
// 		Data:     fmt.Sprint(data),
// 	}
// 	recordJSON, err := json.Marshal(record)
// 	if err != nil {
// 		return err
// 	}

// 	return ctx.GetStub().PutState(id, recordJSON)
// }

// ReadRecordbyData returns the record stored in the world state with given id.
func (s *SmartContract) ReadRecordbyData(ctx contractapi.TransactionContextInterface, data string) (*Record, error) {
	record, err := getQueryResultForQueryString(ctx, fmt.Sprintf(`{"selector":{"data":"%s"}}`, data))
	if err != nil {
		return nil, fmt.Errorf("failed to read from world state: %v", err)
	}
	if record == nil {
		return nil, fmt.Errorf("the record %s does not exist", data)
	}

	return record[0], nil
}

// DeleteRecord deletes an given record from the world state.
func (s *SmartContract) DeleteRecord(ctx contractapi.TransactionContextInterface, id string) error {
	exists, err := s.RecordExists(ctx, id)
	if err != nil {
		return err
	}
	if !exists {
		return fmt.Errorf("the record %s does not exist", id)
	}

	return ctx.GetStub().DelState(id)
}

// RecordExists returns true when record with given ID exists in world state
func (s *SmartContract) RecordExists(ctx contractapi.TransactionContextInterface, id string) (bool, error) {
	recordJSON, err := ctx.GetStub().GetState(id)
	if err != nil {
		return false, fmt.Errorf("failed to read from world state: %v", err)
	}

	return recordJSON != nil, nil
}

// This is an example of a parameterized query where the query logic is baked into the chaincode,
// and accepting a single query parameter (owner).
// Only available on state databases that support rich query (e.g. CouchDB)
// Example: Parameterized rich query
func (s *SmartContract) QueryRecordsByPrevious(ctx contractapi.TransactionContextInterface, id string) ([]*Record, error) {
	queryString := fmt.Sprintf(`{"selector":{"Previous":"%s"}}`, id)
	return getQueryResultForQueryString(ctx, queryString)
}

// getQueryResultForQueryString executes the passed in query string.
// The result set is built and returned as a byte array containing the JSON results.
func getQueryResultForQueryString(ctx contractapi.TransactionContextInterface, queryString string) ([]*Record, error) {
	resultsIterator, err := ctx.GetStub().GetQueryResult(queryString)
	if err != nil {
		return nil, err
	}
	defer resultsIterator.Close()

	return constructQueryResponseFromIterator(resultsIterator)
}

func constructQueryResponseFromIterator(resultsIterator shim.StateQueryIteratorInterface) ([]*Record, error) {
	var records []*Record
	for resultsIterator.HasNext() {
		queryResult, err := resultsIterator.Next()
		if err != nil {
			return nil, err
		}
		var record Record
		err = json.Unmarshal(queryResult.Value, &record)
		if err != nil {
			return nil, err
		}
		records = append(records, &record)
	}

	return records, nil
}

// Forward search from the head of graph
func (s *SmartContract) ForwardSearch(ctx contractapi.TransactionContextInterface, id string, seq int) ([]*Record, error) {

	// var searchQue LinkedQueue
	var records []*Record

	startTime := time.Now().UnixNano()
	queryString := id

	// searchQue.Add(id)
	for true {
		// println("queue size = ", searchQue.Size())
		queryStringForSubmit := GetStringForSelctorMemberListFromString(queryString)
		// currentID := searchQue.Peek().(string)
		// queryString := fmt.Sprintf(`{"selector":{"previous":"%s"}}`, currentID)
		// println(queryString)
		resultsIterator, err1 := ctx.GetStub().GetQueryResult(queryStringForSubmit)
		if err1 != nil {
			return nil, err1
		}
		// println("daozhelema")
		queryString = ""
		for resultsIterator.HasNext() {
			// println("yes")
			queryResult, err2 := resultsIterator.Next()
			if err2 != nil {
				return nil, err2
			}
			// println("a,b")
			var record Record
			err2 = json.Unmarshal(queryResult.Value, &record)
			if err2 != nil {
				return nil, err2
			}
			// println("hellojfkldafjljsdkl\n")
			records = append(records, &record)
			if record.ID != "" {
				queryString = queryString + "|" + record.ID
			}
			// searchQue.Add(record.ID)
			// println("new size = ", searchQue.Size())
		}
		defer resultsIterator.Close()
		if queryString == "" {
			break
		}
		// println("jump iterater")
		// searchQue.Remove()ewfffdsafsa
		// if searchQue.Size() == 0 {
		// 	break
		// }

	}

	endTime := time.Now().UnixNano()
	Ms := float64((endTime - startTime) / 1e6)
	fmt.Printf("Forward: Data %d completed in %f ms\n", seq, Ms)
	return records, nil
}

// Backward search from the tail of graph
func (s *SmartContract) BackwardSearch(ctx contractapi.TransactionContextInterface, id string) ([]*Record, error) {

	var records []*Record

	for true {
		recordJSON, err := ctx.GetStub().GetState(id)
		if err != nil {
			return nil, fmt.Errorf("failed to read from world state: %v", err)
		}
		if recordJSON == nil {
			return records, nil
			// return nil, fmt.Errorf("the record %s does not exist", id)
		}

		var record Record
		err = json.Unmarshal(recordJSON, &record)

		if err != nil {
			return nil, err
		}
		// println("hello\nunmarshal id = %s", record.ID)

		records = append(records, &record)
		id = record.Previous
		// println("id = %s", id)
	}

	return records, nil
}

// GetAllRecords returns all records found in world state
func (s *SmartContract) GetAllRecords(ctx contractapi.TransactionContextInterface) ([]*Record, error) {
	// range query with empty string for startKey and endKey does an
	// open-ended query of all records in the chaincode namespace.
	resultsIterator, err := ctx.GetStub().GetStateByRange("", "")
	if err != nil {
		return nil, err
	}
	defer resultsIterator.Close()

	var records []*Record
	for resultsIterator.HasNext() {
		queryResponse, err := resultsIterator.Next()
		if err != nil {
			return nil, err
		}

		var record Record
		err = json.Unmarshal(queryResponse.Value, &record)
		if err != nil {
			return nil, err
		}
		records = append(records, &record)
	}

	return records, nil
}

// search with Tag
func (s *SmartContract) SearchWithTag(ctx contractapi.TransactionContextInterface, tag string) ([]*Record, error) {
	querystring := fmt.Sprintf(`{"selector":{"tag":"%s"}}`, tag)
	resultsIterator, err := ctx.GetStub().GetQueryResult(querystring)
	if err != nil {
		return nil, err
	}
	defer resultsIterator.Close()

	var records []*Record

	for resultsIterator.HasNext() {
		queryResponse, err := resultsIterator.Next()
		if err != nil {
			return nil, err
		}

		var record Record
		err = json.Unmarshal(queryResponse.Value, &record)
		if err != nil {
			return nil, err
		}

		records = append(records, &record)
		// println("unmarshal tag = %s, id = %s \n", record.Tag, record.ID)

	}

	return records, nil

}

// search with different tags
func (s *SmartContract) SearchWithDifferentTag(ctx contractapi.TransactionContextInterface, tags []string) {
	// for each tag, search with tag
	for i := 0; i < len(tags); i++ {
		// measure time
		startTime := time.Now().UnixNano()
		s.SearchWithTag(ctx, tags[i])
		endTime := time.Now().UnixNano()
		// time for this tag
		Ms := float64((endTime - startTime) / 1e6)
		fmt.Printf("Search with tag %s completed in %f ms\n", tags[i], Ms)
	}
	fmt.Sprintln("Direct Search Finished!")
}

func main() {
	recordChaincode, err := contractapi.NewChaincode(&SmartContract{})
	if err != nil {
		log.Panicf("Error creating niaiveTree-basic chaincode: %v", err)
	}

	if err := recordChaincode.Start(); err != nil {
		log.Panicf("Error starting niaiveTree-basic chaincode: %v", err)
	}
}
