# WyrdLang

WyrdLang is a custom hobby programming language that reimagines traditional programming keywords with mystical, fantasy-inspired terminology. It transforms the familiar syntax of modern programming into something that feels like casting spells and weaving enchantments.

## Philosophy

WyrdLang maintains the logic and structure of conventional programming languages while replacing standard keywords with evocative alternatives that invoke a sense of magic and ancient wisdom. The name "Wyrd" comes from Old English, meaning fate or destiny, reflecting the language's blend of old-world mysticism with modern computational logic.

## Keyword Replacements

WyrdLang replaces standard programming keywords with the following alternatives:

| Standard Keyword | WyrdLang Keyword | Meaning |
|-----------------|------------------|---------|
| `AND` | `TOGETHER` | Logical AND operation |
| `CLASS` | `CLAN` | Class definition |
| `ELSE` | `OTHERWISE` | Else condition |
| `FALSE` | `NAY` | Boolean false |
| `FUN` | `SPELL` | Function definition |
| `FOR` | `CYCLE` | For loop |
| `IF` | `WHEN` | If condition |
| `NIL` | `EMPTINESS` | Null/nil value |
| `OR` | `EITHER` | Logical OR operation |
| `PRINT` | `CAST` | Print/output statement |
| `RETURN` | `MANIFEST` | Return statement |
| `SUPER` | `ELDER` | Super/parent class reference |
| `THIS` | `THINE` | This/self reference |
| `TRUE` | `AYE` | Boolean true |
| `VAR` | `ENCHANT` | Variable declaration |
| `WHILE` | `ASLONGAS` | While loop |

## Example Code

Here's what a simple program looks like in WyrdLang:

```
enchant a = 0;
enchant temp;

cycle (enchant b = 1; a < 10000; b = temp + b) {
  cast a;
  temp = a;
  a = b;
}
```
```
spell makeCounter() {
  enchant i = 0;
  spell count() {
    i = i + 1;
    cast i;
  }

  manifest count;
}

enchant counter = makeCounter();
counter(); // "1".
counter(); // "2".
```

## Purpose

WyrdLang is a hobby project created for fun and experimentation. It demonstrates how programming languages are ultimately just human-readable abstractions over computational logic, and that the choice of keywords is arbitrary—what matters is consistency and clarity.

## Status

This is a hobby project and is not intended for production use. It exists purely for educational purposes, creative expression, and the joy of reimagining how we talk to computers.

## Contributing

Feel free to suggest additional keyword replacements or improvements to make WyrdLang even more mystical and enchanting!

---

*"In WyrdLang, we don't write code—we weave spells."*
