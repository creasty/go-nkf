go-nkf
======

[![Build Status](https://travis-ci.org/creasty/go-nkf.svg?branch=master)](https://travis-ci.org/creasty/go-nkf)

nkf binding for Golang


Usage
-----

### `Convert(str string, options string) (string, error)`

```go
str, err := nkf.Convert("あいうえお０１２３", "-m0Z1 -w --katakana")
if err != nil {
	fmt.Println(str)
	//=> アイウエオ0123
}
```

### `Guess(str string) (Encoding, error)`

```go
str, err := nkf.Guess("abc")
if err != nil {
	fmt.Println(str)
	//=> ASCII
}
```
