# MinTE - The most minimal template engine.

https://github.com/philyg/minte

## Motivation

Sometimes you just need to replace some placeholders in files. A typical example would be configuration files for docker images where you replace placeholders in configuration files with environment variable values upon start.

While there exist tools and/or libraries like _Jinja2_ or _Mustache_, they seem too powerful to _just_ fill in some placeholders. On the other hand, _envsubst_ follows a fixed placeholder format which might work fine for configuration files but not for shell scripts.

This repository therefore presents a very simple C++ based tool to carry out such variable substitutions with selectable delimiters for maximum flexibility.

## Requirements

The project shall implement a tool that:

- Reads text line-wise from the standard input.
- Replaces occurrences of variable references, marked by delimiters, with their environment variable value in the stream.
- Removes delimited comments from the stream.
- Allows changing of the start, end and comment delimiters.
- Allows the user to specify allowed and/or ignored variable names.
- Outputs the edited stream back to standard out.

## Implementation

MinTE (pronounced "minty") is implemented using only C++ standard library features and should therefore compile with a broad range of compilers. However, support for C++11 is needed.

## Usage

### Building

Building should be as easy as `make`. Run `make help` to get more information:

````
To compile dynamically linked use:  make
To compile statically  linked use:  make CFLAGS=-static
To install to /user/local/bin use:  make install
To install to a custom prefix use:  make install PREFIX=[your prefix]

````

### Running

This is the help text output from the tool:

````
MinTE - The most minimal template engine.
Copyright (c) 2025 Philipp Grassl
https://github.com/philyg/minte

A minimalistic tool to replace placeholders in files with environment variable values.
Reads text lines from stdin and replaces {{ NAME }} by the value of the environment
variable NAME. The start and end delimiters can be set using -s and -e parameters and
are allowed to be equal.

Comments can also be written, which follow the format {{ % This is a comment }} where
the comment marker (% by default) must be the first non-space character after the start
delimiter. The comment marker can be set using the -c parameter.

The replaced variables are filtered as follows:
- If allowed variables are specified (-a), only those variables are replaced
- Additionally, ignored variables (-i) are never replaced

By default, used variables must be defined, otherwise an error is thrown. This
behavior can be changed using the -n switch.

All leading and trailing whitespaces are removed between the start and end delimiter.

Usage: minte [-a VAR1,...] [-i VAR1,...] [-n] [-s value] [-e value] [-c value]

-h            Display this help text.

-a value      Comma-delimited list of allowed variables. Cumulative. Default: none
-i value      Comma-delimited list of ignored variables. Cumulative. Default: none
-n            Replace non-defined variables with an empty string. Default: no

-e value      Set the End-Delimiter to the provided string. Default: }}
-s value      Set the Start-Delimiter to the provided string. Default: {{
-c value      Set the Comment Marker. Default: %
````

### Example

Think of this example docker configuration file:

````
{
    "default-address-pools": [
        {
            "base": "10.0.0.0/8",
            "size": 26
        }
    ]
}
````

We would like to be able to easily set the base and size parameters through environment variables. Now, `{{` and `}}` are not valid JSON, but we choose to use `{:` and `:}` instead to make our intentions more clear to a reader of the file:

````
{
    "default-address-pools": [
        {
            "base": "{: NETBASE :}",
            "size": {: NETSIZE :}{: % This is a comment! :}
        }
    ]
}
````

Piping this file through `NETBASE=172.16.18.0/24 NETSIZE=28 ./minte -s {: -e :}` yields:

````
{
    "default-address-pools": [
        {
            "base": "172.16.18.0/24",
            "size": 28
        }
    ]
}
````
