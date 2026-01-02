# SMC (Super Mega Cool) Client v1.0.0-ALPHA by Mahestro_320

The **SMC client** (_SMCC_) is a **CLI (console)** application which uses commands to connect to an
**SMC server** and request data from it by sending requests.

## How to compile

The client is programmed in **C++ (ISO C++20 Standard)**. To compile it, you can use
**Visual Studio** or the **basic C++ compiler**

> [!WARNING]
> It needs **Boost 1.88.0+ to work**. To do this, **put the Boost include folder in `external`
directory at root**

## Environnement variables

To insert an environnement variable in a string, type <[VARNAME]>

## How to connect

To connect to a server, you can modify the `server.address` and `server.port` keys of the
configuration file **(more explanations below)**

## Configuration file (.ini)

The configuration file is stored at `data\config.ini`. To modify values ​​in the file, you can either
do it through the shell (with the `conf` command) or with a standard text editor

> [!NOTE]
> Booleans are represented with a **0** for **false** and **1** for **true**

Here are the file keys and their descriptions:

```ini
[server]

; This is the address the client will try to connect to (ip or domain name)
address=example.com

; This is the port on which the target server accepts connections
port=500

[client]

; Indicates whether the client should connect when the application is launched
connect_at_launch=1

[shell]

; Indicates whether the output is verbose or not
verbose=1

; Indicates whether the text `<address>:<ip>` before the command line should be displayed
print_addr_prefix=1

[commands]

; The default output for downloaded files
download_output=C:\Users\<USERNAME>\Downloads

; The size of each buffer that is received / sent (maximum is 65535)
download_buffer_size=4096
```

> [!NOTE]
> Each values ​​in the ini file above are examples

## Language file (.smcl)

The **SMCL format** is almost a **programming language** (it uses a lexer and a parser). It
contains all the translations for the client (the keys must matches, otherwise it does nothing),
for example:

```
HEADER;
	.name = "English";
	.locale = "en_US";
	.version = 1.0.0;
	.authors = ["Mahestro"];
END;

SECTION glb;
	SECTION error;
		STRBEGIN "[ERROR] ";

		.memory_error = "memory error: {$}";
		.unhandled_exception = "unhandled exception: {$}";
	END;
END;
```

Each expression must finish with a semicolon `;`.

### Translations

To add a translation, we simply do like some other languages to create a variable:

```
.<translation name> = <translation content>;
```

But we add a point before the name

### Sections

The sections is like an INI file, but this time we can nest them up to **2^64 times** ! Take the
example above: to access the key `unhandled_exception`, we put the name of the sections where the
key is located and we separate them with a point and finally we put the name after:
`glb.error.unhandled_exception`. Each sections must finish with the keyword `END`.

### Header

Each SMCL file **must contain a header**; it indicates the name, the locale, the version and the
authors of the translation. We can't put sections in it or put it in sections, remove or add a
custom key. The header is constant but you can put it in the middle of the file (don't do this
please, it looks like sh*t). The lang name and the locale is a string but the version is 3 numbers,
each one separated with a point. The authors is a list of string separated with a comma, and to
delimit it we use brackets `[]`.

### Strbegin

To avoid redundancy, we can use the keyword `STRBEGIN`. It is very simple: each translation value
after the expression starts with its argument. Take this example from the code above:

```
STRBEGIN "[ERROR] ";

.memory_error = "memory error: {$}";
.unhandled_exception = "unhandled exception: {$}";
```

After the first line, every translation value starts with `[ERROR] `: `memory error: {$}` is now
`[ERROR] memory error: {$}` and `unhandled exception: {$}` is now
`[ERROR] unhandled exception: {$}`. It can also be added to other `STRBEGIN`, for example:

```
STRBEGIN "first strbegin: ";
.test1 = "here is test1 !";

SECTION test_section;
	STRBEGIN "second strbegin: ";
	.test2 = "here is test2 !";
END;

.test3 = "here is test3 !";

```

If we apply `STRBEGIN`, `test1` is now `first strbegin: here is test1 !`, `test2` is now
`first strbegin: second strbegin: here is test2 !` and `test3` is now
`first strbegin: here is test3 !`
