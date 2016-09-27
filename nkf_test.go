package nkf

import (
	"io"
	"os/exec"
	"testing"
)

func TestConvert(t *testing.T) {
	cases := []struct {
		input       string
		output      string
		options     string
		description string
	}{
		{
			input:       "ａｂｃｄＡＢＣＤ０１２３",
			output:      "abcdABCD0123",
			options:     "-m0Z1 -w",
			description: "should replace Japanese-alphanumerics to real alphanumerics",
		},
		{
			input:       "　",
			output:      " ",
			options:     "-m0Z1 -w",
			description: "should replace a Japanese-space to a general space",
		},
		{
			input:       "あいうえお",
			output:      "アイウエオ",
			options:     "-m0Z1 -w --katakana",
			description: "should convert hiragana to katakana form",
		},
	}

	for _, c := range cases {
		actual, err := Convert(c.input, c.options)
		if err != nil {
			t.Error(err)
		}

		if actual != c.output {
			t.Error(c.description)
		}
	}
}

func TestCovertConcurrent(t *testing.T) {
	n := 3
	c := make(chan bool, n)

	for i := 0; i < n; i++ {
		go func() {
			TestConvert(t)
			c <- true
		}()
	}

	for i := 0; i < n; i++ {
		<-c
	}
}

func TestGuess(t *testing.T) {
	t.Skip()
}

func BenchmarkConvertByBinding(b *testing.B) {
	for n := 0; n < b.N; n++ {
		Convert("ａｂｃｄＡＢＣＤ０１２３　あいうえお", "-m0Z1 -w --katakana")
	}
}

func BenchmarkConvertByCli(b *testing.B) {
	for n := 0; n < b.N; n++ {
		nkfCommand("ａｂｃｄＡＢＣＤ０１２３　あいうえお", "-m0Z1 -w --katakana")
	}
}

func nkfCommand(str string, flags ...string) string {
	c := exec.Command("nkf", flags...)
	stdin, err := c.StdinPipe()
	if err != nil {
		stdin.Close()
		return str
	}

	if _, err := io.WriteString(stdin, str); err != nil {
		stdin.Close()
		return str
	}

	stdin.Close()
	b, err := c.Output()
	if err != nil {
		return str
	}

	return string(b[:])
}
