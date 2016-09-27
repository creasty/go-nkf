go-nkf
======

[![Build Status](https://travis-ci.org/creasty/go-nkf.svg?branch=master)](https://travis-ci.org/creasty/go-nkf)

nkf binding for Golang


Why
---

It's almost 1,000 times faster than `exec.Command`.

```sh
$ go test -bench=.
PASS
BenchmarkConvertByBinding-4       200000              5742 ns/op
BenchmarkConvertByCli-4              300           5103664 ns/op
ok      github.com/creasty/go-nkf       3.075s
```


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
