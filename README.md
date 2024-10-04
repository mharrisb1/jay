# 🐦 Jay - JSON utils

Collection of utilities for working with JSON

> [!WARNING]
> WIP adn probably buggy

## Help

```sh
🐦 Jay - JSON utils

Usage: build/app [OPTIONS] [COMMAND]

Commands:
  pp		Pretty print (default)
Options:
  -v, --verbose	Increase logging verbosity
  -q, --quiet	Decrease logging verbosity
  -h, --help	Print this message
```

## Usage

### Pretty print

Pretty pring a JSON object

```sh
echo '{"name": "John Doe", "age": 30.0, "ethnicity": null, "is_student": false, "courses": ["Math", "Science"], "address": {"street": "123 Main St", "city": "Anytown"}}' | jay pp
{
  "name": "John Doe",
  "age": 30.000000,
  "ethnicity": null,
  "is_student": false,
  "courses": [
    "Math",
    "Science"
  ],
  "address": {
    "street": "123 Main St",
    "city": "Anytown"
  }
}
```
