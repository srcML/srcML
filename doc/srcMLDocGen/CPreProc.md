
#C Preprocessor srcML

<table>
    <thead>
        <tr>
            <th>Language Element</th>
            <th>Elements</th>
            <th>Subelements</th>
        </tr>
    </thead>
    <tbody>
</tbody><thead>
        <tr>
            <th align="left" colspan="3"><b>C Preprocessor</b></th>
        </tr>
        </thead><tbody>

        <tr>
            <td><a href="#define">#define</a> </td>
            <td>&lt;cpp:define&gt;</td>
            <td>&lt;cpp:directive&gt;, &lt;cpp:macro&gt;, &lt;cpp:value&gt;</td>
        </tr>

        <tr>
            <td><a href="#undef">#undef</a> </td>
            <td>&lt;cpp:undef&gt;</td>
            <td>&lt;cpp:directive&gt;, &lt;name&gt;</td>
        </tr>

        <tr>
            <td><a href="#if">#if</a> </td>
            <td>&lt;cpp:if&gt;</td>
            <td>&lt;cpp:directive&gt;, &lt;expr&gt;</td>
        </tr>

        <tr>
            <td><a href="#ifdef">#ifdef</a> </td>
            <td>&lt;cpp:ifdef&gt;</td>
            <td>&lt;cpp:directive&gt;, &lt;expr&gt;</td>
        </tr>

        <tr>
            <td><a href="#ifndef">#ifndef</a> </td>
            <td>&lt;cpp:ifndef&gt;</td>
            <td>&lt;cpp:directive&gt;, &lt;expr&gt;</td>
        </tr>

        <tr>
            <td><a href="#else">#else</a> </td>
            <td>&lt;cpp:else&gt;</td>
            <td>&lt;cpp:directive&gt;</td>
        </tr>

        <tr>
            <td><a href="#elif">#elif</a> </td>
            <td>&lt;cpp:elif&gt;</td>
            <td>&lt;cpp:directive&gt;, &lt;expr&gt;</td>
        </tr>

        <tr>
            <td><a href="#endif">#endif</a> </td>
            <td>&lt;cpp:if&gt;</td>
            <td>&lt;cpp:directive&gt;</td>
        </tr>

        <tr>
            <td><a href="#include">#include</a> </td>
            <td>&lt;cpp:include&gt;</td>
            <td>&lt;cpp:directive&gt;, &lt;cpp:file&gt;</td>
        </tr>

        <tr>
            <td><a href="#pragma">#pragma</a> </td>
            <td>&lt;cpp:pragma&gt;</td>
            <td>&lt;cpp:directive&gt;</td>
        </tr>

        <tr>
            <td><a href="#error">#error</a> </td>
            <td>&lt;cpp:error&gt;</td>
            <td>&lt;cpp:value&gt;</td>
        </tr>

        <tr>
            <td><a href="#line">#line</a> </td>
            <td>&lt;cpp:line&gt;</td>
            <td>&lt;cpp:directive&gt;, &lt;cpp:number&gt;</td>
        </tr>
</tbody></table>

## #define
### Element
* **&lt;cpp:define&gt;** 

### SubElements
* **&lt;cpp:directive&gt;** 
* **&lt;cpp:macro&gt;** 
* **&lt;cpp:value&gt;** 


### Examples

#### Trivial #define

```C
#define WIDTH 80
#define LENGTH ( WIDTH + 10 )

```
```XML
<cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>WIDTH</name></cpp:macro> <cpp:value>80</cpp:value></cpp:define>
<cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>LENGTH</name></cpp:macro> <cpp:value>( WIDTH + 10 )</cpp:value></cpp:define>

```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of the uses of `#define`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:define`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all macro definitions of a macro with the name `LENGTH`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:define[cpp:macro/src:name[. = 'LENGTH']]`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all values of all macro definitions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:define/cpp:value`<br/>
<br/>
#### #define used like a function

```C
#define multiply( f1, f2 ) ( f1 * f2 )

```
```XML
<cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>multiply</name><parameter_list>( <param><type><name>f1</name></type></param>, <param><type><name>f2</name></type></param> )</parameter_list></cpp:macro> <cpp:value>( f1 * f2 )</cpp:value></cpp:define>

```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all macro definitions which have parameters.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:define[src:parameter_list]`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Get the names of all parameters within all macro declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:define/src:parameter_list/src:param`<br/>
<br/>
#### #define a variadic macro

```C
#define eprintf(format, ...) fprintf (stderr, format, __VA_ARGS__)

```
```XML
<cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>eprintf</name><parameter_list>(<param><type><name>format</name></type></param>, <param><type><type:modifier>...</type:modifier></type></param>)</parameter_list></cpp:macro> <cpp:value>fprintf (stderr, format, __VA_ARGS__)</cpp:value></cpp:define>

```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all variadic macro definitions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:define[src:parameter_list[ .//type:modifier = '...']]`<br/>
<br/>
## #undef
### Element
* **&lt;cpp:undef&gt;** 

### SubElements
* **&lt;cpp:directive&gt;** 
* **&lt;name&gt;** 


### Example

```C
#define multiply( f1, f2 ) ( f1 * f2 )
#undef multiply

```
```XML
<cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>multiply</name><parameter_list>( <param><type><name>f1</name></type></param>, <param><type><name>f2</name></type></param> )</parameter_list></cpp:macro> <cpp:value>( f1 * f2 )</cpp:value></cpp:define>
<cpp:undef>#<cpp:directive>undef</cpp:directive> <name>multiply</name></cpp:undef>

```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#undef`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:undef`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find the name of all undefined macros.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:undef/src:name`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all macro names which are defined and undefined.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:undef/src:name[.= //cpp:define/src:macro/src:name]`<br/>
<br/>
## #if
### Element
* **&lt;cpp:if&gt;** 

### SubElements
* **&lt;cpp:directive&gt;** 
* **&lt;expr&gt;** 


### Example

```C
#if DLEVEL > 5

```
```XML
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>DLEVEL</name> <op:operator>&gt;</op:operator> <lit:literal type="number">5</lit:literal></expr></cpp:if>

```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#if`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:if`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all conditions used by `#if`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:if/src:expr`<br/>
<br/>
## #ifdef
### Element
* **&lt;cpp:ifdef&gt;** 

### SubElements
* **&lt;cpp:directive&gt;** 
* **&lt;expr&gt;** 


### Example

```C
#ifdef NDEBUG
    if(x >20) {
        abort();
    }
#endif
```
```XML
<cpp:ifdef>#<cpp:directive>ifdef</cpp:directive> <name>NDEBUG</name></cpp:ifdef>
    <if>if<condition>(<expr><name>x</name> <op:operator>&gt;</op:operator><lit:literal type="number">20</lit:literal></expr>)</condition><then> <block>{
        <expr_stmt><expr><call><name>abort</name><argument_list>()</argument_list></call></expr>;</expr_stmt>
    }
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif></block></then></if>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#ifdef`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:ifdef`<br/>
<br/>
## #ifndef
### Element
* **&lt;cpp:ifndef&gt;** 

### SubElements
* **&lt;cpp:directive&gt;** 
* **&lt;expr&gt;** 


### Example

```C
#ifndef test
#define final
#endif
```
```XML
<cpp:ifndef>#<cpp:directive>ifndef</cpp:directive> <name>test</name></cpp:ifndef>
<cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>final</name></cpp:macro></cpp:define>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#ifndef`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:ifndef`<br/>
<br/>
## #else
### Element
* **&lt;cpp:else&gt;** 

### SubElement
* **&lt;cpp:directive&gt;** 


### Example

```C
#if DLEVEL > 5
    #define SIGNAL  1
#else
    #define SIGNAL  0
#endif
```
```XML
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>DLEVEL</name> <op:operator>&gt;</op:operator> <lit:literal type="number">5</lit:literal></expr></cpp:if>
    <cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>SIGNAL</name></cpp:macro>  <cpp:value>1</cpp:value></cpp:define>
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
    <cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>SIGNAL</name></cpp:macro>  <cpp:value>0</cpp:value></cpp:define>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#else`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:else`<br/>
<br/>
## #elif
### Element
* **&lt;cpp:elif&gt;** 

### SubElements
* **&lt;cpp:directive&gt;** 
* **&lt;expr&gt;** 


### Example

```C
#if DLEVEL == 0
    #define STACK 0
#elif DLEVEL == 1
    #define STACK 100
#elif DLEVEL > 5
    display( debugptr );
#else
    #define STACK 200
#endif
```
```XML
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>DLEVEL</name> <op:operator>==</op:operator> <lit:literal type="number">0</lit:literal></expr></cpp:if>
    <cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>STACK</name></cpp:macro> <cpp:value>0</cpp:value></cpp:define>
<cpp:elif>#<cpp:directive>elif</cpp:directive> <expr><name>DLEVEL</name> <op:operator>==</op:operator> <lit:literal type="number">1</lit:literal></expr></cpp:elif>
    <cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>STACK</name></cpp:macro> <cpp:value>100</cpp:value></cpp:define>
<cpp:elif>#<cpp:directive>elif</cpp:directive> <expr><name>DLEVEL</name> <op:operator>&gt;</op:operator> <lit:literal type="number">5</lit:literal></expr></cpp:elif>
    <expr_stmt><expr><call><name>display</name><argument_list>( <argument><expr><name>debugptr</name></expr></argument> )</argument_list></call></expr>;
<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
    <cpp:define>#<cpp:directive>define</cpp:directive> <cpp:macro><name>STACK</name></cpp:macro> <cpp:value>200</cpp:value></cpp:define>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif></expr_stmt>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#elif`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:elif`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all conditional expressions used with `#elif`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:elif/src:expr`<br/>
<br/>
## #endif
### Element
* **&lt;cpp:if&gt;** 

### SubElement
* **&lt;cpp:directive&gt;** 


### Example

```C
#endif
```
```XML
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#endif`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:endif`<br/>
<br/>
## #include
### Element
* **&lt;cpp:include&gt;** 

### SubElements
* **&lt;cpp:directive&gt;** 
* **&lt;cpp:file&gt;** 


### Example

```C
#include <FileName>
#include "fileName"
```
```XML
<cpp:include>#<cpp:directive>include</cpp:directive> <cpp:file>&lt;FileName&gt;</cpp:file></cpp:include>
<cpp:include>#<cpp:directive>include</cpp:directive> <cpp:file><lit:literal type="string">&quot;fileName&quot;</lit:literal></cpp:file></cpp:include>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all includes.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:include`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all include that use quotations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:include[cpp:file/lit:literal[@type ='string']]`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find the names of all included files.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:include/cpp:file//text()`<br/>
<br/>
## #pragma
### Element
* **&lt;cpp:pragma&gt;** 

### SubElement
* **&lt;cpp:directive&gt;** 


### Example

```C
// #pragma example
#pragma warning( once : 4385 )
#pragma ms_struct on
```
```XML
<comment type="line">// #pragma example</comment>
<cpp:pragma>#<cpp:directive>pragma</cpp:directive> warning( once : 4385 )</cpp:pragma>
<cpp:pragma>#<cpp:directive>pragma</cpp:directive> ms_struct on</cpp:pragma>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#pragma`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:pragma`<br/>
<br/>
## #error
### Element
* **&lt;cpp:error&gt;** 

### SubElement
* **&lt;cpp:value&gt;** 


### Example

```C
// #error message example
#if !defined(__cplusplus)
#error "C++ compiler required."
#endif
```
```XML
<comment type="line">// #error message example</comment>
<cpp:if>#<cpp:directive>if</cpp:directive> <expr><op:operator>!</op:operator><call><name>defined</name><argument_list>(<argument><expr><name>__cplusplus</name></expr></argument>)</argument_list></call></expr></cpp:if>
<cpp:error>#<cpp:directive>error</cpp:directive> &quot;C++ compiler required.&quot;</cpp:error>
<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#error`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:error`<br/>
<br/>
## #line
### Element
* **&lt;cpp:line&gt;** 

### SubElements
* **&lt;cpp:directive&gt;** 
* **&lt;cpp:number&gt;** 


### Example

```C
// #line directive example
#line 151 "copy.c"
```
```XML
<comment type="line">// #line directive example</comment>
<cpp:line>#<cpp:directive>line</cpp:directive> <cpp:number><lit:literal type="number">151</lit:literal></cpp:number> <cpp:file><lit:literal type="string">&quot;copy.c&quot;</lit:literal></cpp:file></cpp:line>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of `#line`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//cpp:line`<br/>
<br/>