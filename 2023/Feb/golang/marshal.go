package main

import (
	"encoding/json"
	"fmt"
)

// define struct with json fields
type SampleSt struct {
	A int    `json:"A"`
	B string `json:"B"`
}

// define print function for SampleSt

func (s *SampleSt) print() {
	fmt.Printf("a: %d, ", s.A)
	fmt.Printf("b: %s", s.B)
	fmt.Printf("\n")
}

// marshal SampleSt to json

func (s *SampleSt) MarshalJSON() ([]byte, error) {
	/*return json.Marshal(SampleSt{
		A: s.A,
		B: s.B,
	})*/
	return json.Marshal(*s)
}

// convert json string to SampleSt

func (s *SampleSt) UnmarshalJSON(b []byte) error {
	// unmarshal json string to SampleSt
	type TMPSampleSt SampleSt
	var tmp TMPSampleSt
	err := json.Unmarshal(b, &tmp)
	if err != nil {
		return err
	}
	*s = SampleSt(tmp)
	return nil
}

func (s *SampleSt) UnmarshalJSON2(b []byte) error {
	// unmarshal json string to SampleSt
	var tmp SampleSt
	err := json.Unmarshal(b, &tmp)
	if err != nil {
		return err
	}
	*s = SampleSt(tmp)
	return nil
}

func main() {

	// define struct
	s1 := SampleSt{1, "hello"}
	s2 := SampleSt{2, "world"}

	// define print function
	s1.print()
	s2.print()
	fmt.Println("In JSON format: ")
	jsonData, _ := s2.MarshalJSON()
	fmt.Println(string(jsonData))

	// unmarshal jsonData to SampleSt
	s3 := SampleSt{3, "Data"}
	s3.print()
	err := s3.UnmarshalJSON2(jsonData)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("Back from JSON: ")
		s3.print()
	}

}
