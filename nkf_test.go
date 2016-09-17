package nkf

import (
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
