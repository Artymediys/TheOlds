package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"strings"
)

func Map2String(m map[string]int) string {
	b := new(bytes.Buffer)
	for key, value := range m {
		_, err := fmt.Fprintf(b, "%s – %d\n", key, value)
		if err != nil{
			panic(err)
		}
	}
	return b.String()
}

func WordCounter(s string) string {
	mAp := make(map[string]int)
	sliced := strings.Split(s, " ")
	for _, v := range sliced{
		mAp[v] += 1
	}
	return Map2String(mAp)
}

func main() {
	data, err := ioutil.ReadFile("input.txt")
	if err != nil{
		panic(err)
	}
	data = []byte(WordCounter(string(data)))
	err = ioutil.WriteFile("output.txt", data, 0644)
	if err != nil{
		panic(err)
	}
}