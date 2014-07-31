
#C srcML

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
            <th align="left" colspan="3"><b>Statements</b></th>
        </tr>
        </thead><tbody>

        <tr>
            <td><a href="#if-statement">if</a> </td>
            <td>&lt;if&gt;</td>
            <td>&lt;condition&gt;, &lt;then&gt;, &lt;else&gt;, &lt;elseif&gt;</td>
        </tr>

        <tr>
            <td><a href="#while-statement">while</a> </td>
            <td>&lt;while&gt;</td>
            <td>&lt;condition&gt;, &lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#do-while-statement">do</a> </td>
            <td>&lt;do&gt;</td>
            <td>&lt;block&gt;, &lt;condition&gt;</td>
        </tr>

        <tr>
            <td><a href="#switch-statement">switch</a> </td>
            <td>&lt;switch&gt;</td>
            <td>&lt;case&gt;, &lt;condition&gt;, &lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#case-statement">case</a> </td>
            <td>&lt;case&gt;</td>
            <td>&lt;expr&gt;</td>
        </tr>

        <tr>
            <td><a href="#default-statement">default</a> </td>
            <td>&lt;default&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#for-statement">for</a> </td>
            <td>&lt;for&gt;</td>
            <td>&lt;init&gt;, &lt;condition&gt;, &lt;incr&gt;</td>
        </tr>

        <tr>
            <td><a href="#break-statement">break</a> </td>
            <td>&lt;break&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#continue-statement">continue</a> </td>
            <td>&lt;continue&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#return-statement">return</a> </td>
            <td>&lt;return&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#block-statement">block</a> </td>
            <td>&lt;block&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#label-statement">label</a> </td>
            <td>&lt;label&gt;</td>
            <td>&lt;name&gt;</td>
        </tr>

        <tr>
            <td><a href="#goto-statement">goto</a> </td>
            <td>&lt;goto&gt;</td>
            <td>&lt;name&gt;</td>
        </tr>

        <tr>
            <td><a href="#empty-statement">empty statement</a> </td>
            <td>&lt;empty_stmt&gt;</td>
            <td></td>
        </tr>
</tbody><thead>
        <tr>
            <th align="left" colspan="3"><b>Specifiers</b></th>
        </tr>
        </thead><tbody>

        <tr>
            <td><a href="#const">const</a> </td>
            <td>&lt;specifier&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#volatile">volatile</a> </td>
            <td>&lt;specifier&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#restrict">restrict</a> </td>
            <td>&lt;specifier&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#auto">auto</a> </td>
            <td>&lt;specifier&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#register">register</a> </td>
            <td>&lt;name&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#extern-block">extern block</a> </td>
            <td>&lt;extern&gt;</td>
            <td>&lt;lit:literal&gt;, &lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#extern-specifier">extern specifier</a> </td>
            <td>&lt;specifier&gt;</td>
            <td>&lt;lit:literal&gt;, &lt;decl_stmt&gt;, &lt;function_stmt&gt;, &lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#static">static</a> </td>
            <td>&lt;specifier&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#inline">inline</a> </td>
            <td>&lt;specifier&gt;</td>
            <td></td>
        </tr>
</tbody><thead>
        <tr>
            <th align="left" colspan="3"><b>Declarations/Definitions</b></th>
        </tr>
        </thead><tbody>

        <tr>
            <td><a href="#variable-declaration-statement">variable declaration statement</a> </td>
            <td>&lt;decl_stmt&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#variable-declaration">variable declaration</a> </td>
            <td>&lt;decl&gt;</td>
            <td>&lt;init&gt;</td>
        </tr>

        <tr>
            <td><a href="#function-declaration">function declaration</a> </td>
            <td>&lt;function_decl&gt;</td>
            <td>&lt;name&gt;, &lt;parameter_list&gt;, &lt;param&gt;</td>
        </tr>

        <tr>
            <td><a href="#function-definition">function definition</a> </td>
            <td>&lt;function&gt;</td>
            <td>&lt;name&gt;, &lt;parameter_list&gt;, &lt;param&gt;</td>
        </tr>

        <tr>
            <td><a href="#pointer-declaration">pointer declaration</a> </td>
            <td>&lt;type:modifier&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#typedef">typedef</a> </td>
            <td>&lt;typedef&gt;</td>
            <td>&lt;type&gt;, &lt;name&gt;, &lt;function_decl&gt;</td>
        </tr>

        <tr>
            <td><a href="#array-declaration">array declaration</a> </td>
            <td>&lt;decl&gt;</td>
            <td>&lt;name&gt;, &lt;index&gt;</td>
        </tr>

        <tr>
            <td><a href="#array-initialization">array initialization</a> </td>
            <td>&lt;init&gt;</td>
            <td>&lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#bitfield">bitfield</a> </td>
            <td>&lt;range&gt;</td>
            <td>&lt;expr&gt;, &lt;lit:literal&gt;</td>
        </tr>
</tbody><thead>
        <tr>
            <th align="left" colspan="3"><b>Struct, Union and Enum</b></th>
        </tr>
        </thead><tbody>

        <tr>
            <td><a href="#struct-declaration">struct declaration</a> </td>
            <td>&lt;struct_decl&gt;</td>
            <td>&lt;name&gt;</td>
        </tr>

        <tr>
            <td><a href="#struct-definition">struct definition</a> </td>
            <td>&lt;struct&gt;</td>
            <td>&lt;name&gt;, &lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#struct-initialization">struct initialization</a> </td>
            <td>&lt;decl_stmt&gt;</td>
            <td>&lt;decl&gt;, &lt;init&gt;, &lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#enum-definition">enum definition</a> </td>
            <td>&lt;enum&gt;</td>
            <td>&lt;name&gt;, &lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#union-declaration">union declaration</a> </td>
            <td>&lt;union_decl&gt;</td>
            <td>&lt;name&gt;, &lt;block&gt;</td>
        </tr>

        <tr>
            <td><a href="#union-definition">union definition</a> </td>
            <td>&lt;union&gt;</td>
            <td>&lt;name&gt;, &lt;block&gt;</td>
        </tr>
</tbody><thead>
        <tr>
            <th align="left" colspan="3"><b>Expression</b></th>
        </tr>
        </thead><tbody>

        <tr>
            <td><a href="#function-call">Function Call</a> </td>
            <td>&lt;call&gt;</td>
            <td>&lt;name&gt;, &lt;argument_list&gt;, &lt;argument&gt;</td>
        </tr>

        <tr>
            <td><a href="#array-indexing">array indexing</a> </td>
            <td>&lt;name&gt;</td>
            <td>&lt;name&gt;, &lt;index&gt;</td>
        </tr>

        <tr>
            <td><a href="#dereference-member-access">dereference member access</a> </td>
            <td>&lt;expr&gt;</td>
            <td>&lt;name&gt;, &lt;op:operator&gt;</td>
        </tr>

        <tr>
            <td><a href="#member-access">member access</a> </td>
            <td>&lt;expr&gt;</td>
            <td>&lt;name&gt;, &lt;op:operator&gt;</td>
        </tr>
</tbody><thead>
        <tr>
            <th align="left" colspan="3"><b>Other</b></th>
        </tr>
        </thead><tbody>

        <tr>
            <td><a href="#cast">cast</a> </td>
            <td>&lt;op:operator&gt;</td>
            <td>&lt;name&gt;</td>
        </tr>

        <tr>
            <td><a href="#sizeof">sizeof</a> </td>
            <td>&lt;sizeof&gt;</td>
            <td>&lt;argument_list&gt;, &lt;argument&gt;</td>
        </tr>

        <tr>
            <td><a href="#_alignas">_Alignas</a> </td>
            <td>&lt;alignas&gt;</td>
            <td>&lt;argument_list&gt;, &lt;argument&gt;</td>
        </tr>

        <tr>
            <td><a href="#_alignof">_Alignof</a> </td>
            <td>&lt;alignof&gt;</td>
            <td>&lt;argument_list&gt;, &lt;argument&gt;</td>
        </tr>

        <tr>
            <td><a href="#_atomic">_Atomic</a> </td>
            <td>&lt;atomic&gt;</td>
            <td>&lt;argument_list&gt;, &lt;argument&gt;</td>
        </tr>

        <tr>
            <td><a href="#_generic">_Generic</a> </td>
            <td>&lt;generic_selection&gt;</td>
            <td>&lt;selector&gt;, &lt;association_list&gt;, &lt;association&gt;</td>
        </tr>

        <tr>
            <td><a href="#_noreturn">_Noreturn</a> </td>
            <td>&lt;specifier&gt;</td>
            <td></td>
        </tr>

        <tr>
            <td><a href="#_static_assert">_Static_assert</a> </td>
            <td>&lt;assert type="static"&gt;</td>
            <td>&lt;argument_list&gt;, &lt;argument&gt;</td>
        </tr>

        <tr>
            <td><a href="#_thread_local">_Thread_local</a> </td>
            <td>&lt;specifier&gt;</td>
            <td></td>
        </tr>
</tbody><thead>
        <tr>
            <th align="left" colspan="3"><b>Operators</b></th>
        </tr>
        </thead><tbody>

        <tr>
            <td><a href="#operators">operators</a> </td>
            <td>&lt;op:operator&gt;</td>
            <td></td>
        </tr>
</tbody></table>

## if Statement
### Element
* **&lt;if&gt;** 

### SubElements
* **&lt;condition&gt;** 
* **&lt;then&gt;** 
* **&lt;else&gt;** 
* **&lt;elseif&gt;** 


### Examples

#### 1

```C
if(x > 5)
	y+=4
```
```XML
<if>if<condition>(<expr><name>x</name> <op:operator>&gt;</op:operator> <lit:literal type="number">5</lit:literal></expr>)</condition><then>
	<expr_stmt><expr><name>y</name><op:operator>+=</op:operator><lit:literal type="number">4</lit:literal></expr></expr_stmt></then></if>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all if statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:if`<br/>
<br/>
#### 2

```C
if ( i > 0 ) {
    y = x / i;
}

```
```XML
<if>if <condition>( <expr><name>i</name> <op:operator>&gt;</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition><then> <block>{
    <expr_stmt><expr><name>y</name> <op:operator>=</op:operator> <name>x</name> <op:operator>/</op:operator> <name>i</name></expr>;</expr_stmt>
}</block></then></if>

```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all conditions of all if statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:if/src:condition`<br/>
<br/>
#### 3

```C
if ( i > 0 )
    y = x / i;
else 
{
    x = i;
    y = f( x );
}
```
```XML
<if>if <condition>( <expr><name>i</name> <op:operator>&gt;</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition><then>
    <expr_stmt><expr><name>y</name> <op:operator>=</op:operator> <name>x</name> <op:operator>/</op:operator> <name>i</name></expr>;</expr_stmt></then>
<else>else 
<block>{
    <expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <name>i</name></expr>;</expr_stmt>
    <expr_stmt><expr><name>y</name> <op:operator>=</op:operator> <call><name>f</name><argument_list>( <argument><expr><name>x</name></expr></argument> )</argument_list></call></expr>;</expr_stmt>
}</block></else></if>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all else statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:else`<br/>
<br/>
#### 4

```C
if ( i > 0 )           /* Without braces */
    if ( j > i )
        x = j;
    else
        x = i;
```
```XML
<if>if <condition>( <expr><name>i</name> <op:operator>&gt;</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition><then>           <comment type="block">/* Without braces */</comment>
    <if>if <condition>( <expr><name>j</name> <op:operator>&gt;</op:operator> <name>i</name></expr> )</condition><then>
        <expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <name>j</name></expr>;</expr_stmt></then>
    <else>else
        <expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <name>i</name></expr>;</expr_stmt></else></if></then></if>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all nested if statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:if/src:then//src:if`<br/>
<br/>
#### 5

```C
if ( i > 0 ) 
{                      /* With braces */
    if ( j > i )
        x = j;
}
else
    x = i;
```
```XML
<if>if <condition>( <expr><name>i</name> <op:operator>&gt;</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition><then> 
<block>{                      <comment type="block">/* With braces */</comment>
    <if>if <condition>( <expr><name>j</name> <op:operator>&gt;</op:operator> <name>i</name></expr> )</condition><then>
        <expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <name>j</name></expr>;</expr_stmt></then></if>
}</block></then>
<else>else
    <expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <name>i</name></expr>;</expr_stmt></else></if>
```


#### 6

```C
if ( i > 0 ) 
{
 	x = i;   
}
else if( i < 0)
	x = q;
else 
    x = j;
    
```
```XML
<if>if <condition>( <expr><name>i</name> <op:operator>&gt;</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition><then> 
<block>{
 	<expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <name>i</name></expr>;</expr_stmt>   
}</block></then>
<elseif>else <if>if<condition>( <expr><name>i</name> <op:operator>&lt;</op:operator> <lit:literal type="number">0</lit:literal></expr>)</condition><then>
	<expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <name>q</name></expr>;</expr_stmt></then></if></elseif>
<else>else 
    <expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <name>j</name></expr>;</expr_stmt></else></if>
    
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all else-if statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:elseif`<br/>
<br/>
## while statement
### Element
* **&lt;while&gt;** 

### SubElements
* **&lt;condition&gt;** 
* **&lt;block&gt;** 


### Example

```C
while ( i >= 0 ) 
{
    string1[i] = string2[i];
    i--;
}

```
```XML
<while>while <condition>( <expr><name>i</name> <op:operator>&gt;=</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition> 
<block>{
    <expr_stmt><expr><name><name>string1</name><index>[<expr><name>i</name></expr>]</index></name> <op:operator>=</op:operator> <name><name>string2</name><index>[<expr><name>i</name></expr>]</index></name></expr>;</expr_stmt>
    <expr_stmt><expr><name>i</name><op:operator>--</op:operator></expr>;</expr_stmt>
}</block></while>

```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all while statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:while`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of the conditions of all while statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:while/src:condition`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all while statements which have a condition containing the variable `'X'`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:while[src:condition//src:name = 'X']`<br/>
<br/>
## do while statement
### Element
* **&lt;do&gt;** 

### SubElements
* **&lt;block&gt;** 
* **&lt;condition&gt;** 


### Examples

#### 1

```C
do {
    y = f( x );
    x--;
} while ( x > 0 );
```
```XML
<do>do <block>{
    <expr_stmt><expr><name>y</name> <op:operator>=</op:operator> <call><name>f</name><argument_list>( <argument><expr><name>x</name></expr></argument> )</argument_list></call></expr>;</expr_stmt>
    <expr_stmt><expr><name>x</name><op:operator>--</op:operator></expr>;</expr_stmt>
}</block> while <condition>( <expr><name>x</name> <op:operator>&gt;</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition>;</do>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all do statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:do`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all do while statements which have a condition containing the variable `'Z'`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:do[src:condition//src:name = 'Z']`<br/>
<br/>
#### 2

```C
do x--; while ( x > 0 );

```
```XML
<do>do <expr_stmt><expr><name>x</name><op:operator>--</op:operator></expr>;</expr_stmt> while <condition>( <expr><name>x</name> <op:operator>&gt;</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition>;</do>

```


## switch statement
### Element
* **&lt;switch&gt;** 

### SubElements
* **&lt;case&gt;** 
* **&lt;condition&gt;** 
* **&lt;block&gt;** 


### Examples

#### 1

```C
switch( c ) 
{
    case 'A':
        capa++;
    case 'a':
        lettera++;
    default :
        total++;
}
```
```XML
<switch>switch<condition>( <expr><name>c</name></expr> )</condition> 
<block>{
    <case>case <expr><lit:literal type="char">'A'</lit:literal></expr>:
        <expr_stmt><expr><name>capa</name><op:operator>++</op:operator></expr>;</expr_stmt>
    </case><case>case <expr><lit:literal type="char">'a'</lit:literal></expr>:
        <expr_stmt><expr><name>lettera</name><op:operator>++</op:operator></expr>;</expr_stmt>
    </case><default>default :
        <expr_stmt><expr><name>total</name><op:operator>++</op:operator></expr>;</expr_stmt>
</default>}</block></switch>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all switch statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:switch`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of the conditions of all switch statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:switch/src:condition`<br/>
<br/>
#### 2

```C
switch( i ) 
{
    case -1:
        n++;
        break;
    case 0 :
        z++;
        break;
    case 1 :
        p++;
        break;
}
```
```XML
<switch>switch<condition>( <expr><name>i</name></expr> )</condition> 
<block>{
    <case>case <expr><op:operator>-</op:operator><lit:literal type="number">1</lit:literal></expr>:
        <expr_stmt><expr><name>n</name><op:operator>++</op:operator></expr>;</expr_stmt>
        <break>break;</break>
    </case><case>case <expr><lit:literal type="number">0</lit:literal></expr> :
        <expr_stmt><expr><name>z</name><op:operator>++</op:operator></expr>;</expr_stmt>
        <break>break;</break>
    </case><case>case <expr><lit:literal type="number">1</lit:literal></expr> :
        <expr_stmt><expr><name>p</name><op:operator>++</op:operator></expr>;</expr_stmt>
        <break>break;</break>
</case>}</block></switch>
```


## case statement
### Element
* **&lt;case&gt;** 

### SubElement
* **&lt;expr&gt;** 


### Example

```C
case THREE:
```
```XML
<case>case <expr><name>THREE</name></expr>:</case>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all case statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:case`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all case statements which use the name `'SATURDAY'`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:case[.//src:name = 'SATURDAY']`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all switch statements which have a case that uses the name `'SATURDAY'`.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:switch[src:block/src:case[.//src:name = 'SATURDAY']]`<br/>
<br/>
## default statement
### Element
* **&lt;default&gt;** 


### Example

```C
default:
```
```XML
<default>default:</default>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all default statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:default`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all switch statements which have a case that have a `default:` statement.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:switch[src:block/src:default]`<br/>
<br/>
## for statement
### Element
* **&lt;for&gt;** 

### SubElements
* **&lt;init&gt;** 
* **&lt;condition&gt;** 
* **&lt;incr&gt;** 


### Examples

#### 1

```C
for( ;; ) { }
```
```XML
<for>for( <init>;</init><condition>;</condition> <incr/>) <block>{ }</block></for>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all for statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:for`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all initialization statements from within the control of all for statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:for/src:init`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all for loops which declare a variable with a single character name (e.g. `x`, `i` or `y`).<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:for[string-length(src:init/src:decl/src:name) = 1]`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all for loops which use post decrement as part of the increment expression.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:for[src:incr/src:expr//node()/following-sibling::op:operator[.= '++']]`<br/>
<br/>
#### 2

```C
for (i = 0; i < max; i++ ) 
{
  if ( line[i] == ' ' )
  {
      space++;
  }
  if ( line[i] == '\t' )
  {
      tab++;
  }
}
```
```XML
<for>for (<init><expr><name>i</name> <op:operator>=</op:operator> <lit:literal type="number">0</lit:literal></expr>;</init> <condition><expr><name>i</name> <op:operator>&lt;</op:operator> <name>max</name></expr>;</condition> <incr><expr><name>i</name><op:operator>++</op:operator></expr></incr> ) 
<block>{
  <if>if <condition>( <expr><name><name>line</name><index>[<expr><name>i</name></expr>]</index></name> <op:operator>==</op:operator> <lit:literal type="char">' '</lit:literal></expr> )</condition><then>
  <block>{
      <expr_stmt><expr><name>space</name><op:operator>++</op:operator></expr>;</expr_stmt>
  }</block></then></if>
  <if>if <condition>( <expr><name><name>line</name><index>[<expr><name>i</name></expr>]</index></name> <op:operator>==</op:operator> <lit:literal type="char">'\t'</lit:literal></expr> )</condition><then>
  <block>{
      <expr_stmt><expr><name>tab</name><op:operator>++</op:operator></expr>;</expr_stmt>
  }</block></then></if>
}</block></for>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of the increments expression of all for loops.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:for/src:incr`<br/>
<br/>
#### 3

```C
for(int i =0; i < 10;++i)
	a += x[i];
```
```XML
<for>for(<init><decl><type><name>int</name></type> <name>i</name> <init>=<expr><lit:literal type="number">0</lit:literal></expr></init></decl>;</init> <condition><expr><name>i</name> <op:operator>&lt;</op:operator> <lit:literal type="number">10</lit:literal></expr>;</condition><incr><expr><op:operator>++</op:operator><name>i</name></expr></incr>)
	<expr_stmt><expr><name>a</name> <op:operator>+=</op:operator> <name><name>x</name><index>[<expr><name>i</name></expr>]</index></name></expr>;</expr_stmt></for>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of the conditions of all for loops.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:for/src:condition`<br/>
<br/>
## break statement
### Element
* **&lt;break&gt;** 


### Example

```C
char c;
for(;;) {
    printf_s( "\nPress any key, Q to quit: " );

    // Convert to character value
    scanf_s("%c", &c);
    if (c == 'Q')
        break;
}
```
```XML
<decl_stmt><decl><type><name>char</name></type> <name>c</name></decl>;</decl_stmt>
<for>for(<init>;</init><condition>;</condition><incr/>) <block>{
    <expr_stmt><expr><call><name>printf_s</name><argument_list>( <argument><expr><lit:literal type="string">&quot;\nPress any key, Q to quit: &quot;</lit:literal></expr></argument> )</argument_list></call></expr>;</expr_stmt>

    <comment type="line">// Convert to character value</comment>
    <expr_stmt><expr><call><name>scanf_s</name><argument_list>(<argument><expr><lit:literal type="string">&quot;%c&quot;</lit:literal></expr></argument>, <argument><expr><op:operator>&amp;</op:operator><name>c</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>
    <if>if <condition>(<expr><name>c</name> <op:operator>==</op:operator> <lit:literal type="char">'Q'</lit:literal></expr>)</condition><then>
        <break>break;</break></then></if>
}</block></for>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all break statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:break`<br/>
<br/>
## continue statement
### Element
* **&lt;continue&gt;** 


### Example

```C
while ( i-- > 0 ) 
{
    x = f( i );
    if ( x == 1 ) {
        continue;
    }
    y += x * x;
}
```
```XML
<while>while <condition>( <expr><name>i</name><op:operator>--</op:operator> <op:operator>&gt;</op:operator> <lit:literal type="number">0</lit:literal></expr> )</condition> 
<block>{
    <expr_stmt><expr><name>x</name> <op:operator>=</op:operator> <call><name>f</name><argument_list>( <argument><expr><name>i</name></expr></argument> )</argument_list></call></expr>;</expr_stmt>
    <if>if <condition>( <expr><name>x</name> <op:operator>==</op:operator> <lit:literal type="number">1</lit:literal></expr> )</condition><then> <block>{
        <continue>continue;</continue>
    }</block></then></if>
    <expr_stmt><expr><name>y</name> <op:operator>+=</op:operator> <name>x</name> <op:operator>*</op:operator> <name>x</name></expr>;</expr_stmt>
}</block></while>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all continue statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:continue`<br/>
<br/>
## return statement
### Element
* **&lt;return&gt;** 


### Examples

#### 1

```C
return( s * (long long)s );
```
```XML
<return>return<expr><op:operator>(</op:operator> <name>s</name> <op:operator>*</op:operator> <op:operator>(</op:operator><name>long</name> <name>long</name><op:operator>)</op:operator><name>s</name> <op:operator>)</op:operator></expr>;</return>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all return statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:return`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all functions with more then one return statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function[ count(.//src:return) > 1]`<br/>
<br/>
#### 2

```C
return;
```
```XML
<return>return;</return>
```


## block statement
### Element
* **&lt;block&gt;** 


### Example

```C
 {
    line[i] = x;
    x++;
    i--;
}
```
```XML
 <block>{
    <expr_stmt><expr><name><name>line</name><index>[<expr><name>i</name></expr>]</index></name> <op:operator>=</op:operator> <name>x</name></expr>;</expr_stmt>
    <expr_stmt><expr><name>x</name><op:operator>++</op:operator></expr>;</expr_stmt>
    <expr_stmt><expr><name>i</name><op:operator>--</op:operator></expr>;</expr_stmt>
}</block>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all blocks.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:block`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all variable declaration statements which are children of a block.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:block/src:decl_stmt`<br/>
<br/>
## label statement
### Element
* **&lt;label&gt;** 

### SubElement
* **&lt;name&gt;** 


### Example

```C
stop:
```
```XML
<label><name>stop</name>:</label>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all label statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:label`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all label statements without goto statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:label[src:name[. != //src:goto/src:name]]`<br/>
<br/>
## goto statement
### Element
* **&lt;goto&gt;** 

### SubElement
* **&lt;name&gt;** 


### Example

```C
goto stop;
```
```XML
<goto>goto <name>stop</name>;</goto>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all goto statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:goto`<br/>
<br/>
## empty statement
### Element
* **&lt;empty_stmt&gt;** 


### Example

```C
;
```
```XML
<empty_stmt>;</empty_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all empty statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:empty_stmt`<br/>
<br/>
## const
### Element
* **&lt;specifier&gt;** 


### Examples

#### 1

```C
const int x;
const X y;
```
```XML
<decl_stmt><decl><type><specifier>const</specifier> <name>int</name></type> <name>x</name></decl>;</decl_stmt>
<decl_stmt><decl><type><specifier>const</specifier> <name>X</name></type> <name>y</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all const variable declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl/src:type/src:specifier [.='const']`<br/>
<br/>
#### 2

```C
int const x;
X const y;
```
```XML
<decl_stmt><decl><type><name>int</name> <specifier>const</specifier></type> <name>x</name></decl>;</decl_stmt>
<decl_stmt><decl><type><name>X</name> <specifier>const</specifier></type> <name>y</name></decl>;</decl_stmt>
```


## volatile
### Element
* **&lt;specifier&gt;** 


### Example

```C
volatile int foo; 
int volatile foo;
```
```XML
<decl_stmt><decl><type><specifier>volatile</specifier> <name>int</name></type> <name>foo</name></decl>;</decl_stmt> 
<decl_stmt><decl><type><name>int</name> <specifier>volatile</specifier></type> <name>foo</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all volatile variable declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl/src:type/src:specifier [.='volatile']`<br/>
<br/>
## restrict
### Element
* **&lt;specifier&gt;** 


### Example

```C
restrict double* x;
restrict union Z* x;
```
```XML
<decl_stmt><decl><type><specifier>restrict</specifier> <name>double</name><type:modifier>*</type:modifier></type> <name>x</name></decl>;</decl_stmt>
<decl_stmt><decl><type><specifier>restrict</specifier> union <name>Z</name><type:modifier>*</type:modifier></type> <name>x</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all variables declared with the restrict specifier.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl/src:type/src:specifier [.='restrict']`<br/>
<br/>
## auto
### Element
* **&lt;specifier&gt;** 


### Example

```C
auto int x;
```
```XML
<decl_stmt><decl><type><specifier>auto</specifier> <name>int</name></type> <name>x</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all auto variable declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl/src:type/src:specifier [.='auto']`<br/>
<br/>
## register
### Element
* **&lt;name&gt;** 


### Example

```C
register int x;
register float y;
```
```XML
<decl_stmt><decl><type><name>register</name> <name>int</name></type> <name>x</name></decl>;</decl_stmt>
<decl_stmt><decl><type><name>register</name> <name>float</name></type> <name>y</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all volatile variable declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl/src:type/src:name [.='volatile']`<br/>
<br/>
## extern block
### Element
* **&lt;extern&gt;** 

### SubElements
* **&lt;lit:literal&gt;** 
     * **Note**: This mark up is disabled by default and is enabled using the following command line argument/s with srcML: 
        * --literal
* **&lt;block&gt;** 


### Example

#### Extern Block with Linkage Specifier

```C
extern "C" { }
```
```XML
<extern>extern <lit:literal type="string">&quot;C&quot;</lit:literal> <block>{ }</block></extern>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all extern block variable declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:extern`<br/>
<br/>
## extern specifier
### Element
* **&lt;specifier&gt;** 

### SubElements
* **&lt;lit:literal&gt;** 
     * **Note**: This mark up is disabled by default and is enabled using the following command line argument/s with srcML: 
        * --literal
* **&lt;decl_stmt&gt;** 
* **&lt;function_stmt&gt;** 
* **&lt;block&gt;** 


### Examples

#### External Variable

```C
extern int X;
```
```XML
<decl_stmt><decl><type><specifier>extern</specifier> <name>int</name></type> <name>X</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all variables declared with the specifier extern.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl/src:type/src:specifier [.='extern']`<br/>
<br/>
#### External Structure

```C
extern struct Z X;
```
```XML
<decl_stmt><decl><type><specifier>extern</specifier> struct <name>Z</name></type> <name>X</name></decl>;</decl_stmt>
```


#### External Function

```C
extern void foo();
```
```XML
<function_decl><type><specifier>extern</specifier> <name>void</name></type> <name>foo</name><parameter_list>()</parameter_list>;</function_decl>
```


#### External Function with Linkage Specifier

```C
extern "C" void foo();
```
```XML
<extern>extern <lit:literal type="string">&quot;C&quot;</lit:literal> <function_decl><type><name>void</name></type> <name>foo</name><parameter_list>()</parameter_list>;</function_decl></extern>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all functions declared with the specifier extern.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl/src:type/src:specifier [.='extern']`<br/>
<br/>
#### Extern Structure with Linkage Specifier

```C
extern "C" struct Z X;
```
```XML
<extern>extern <lit:literal type="string">&quot;C&quot;</lit:literal> <decl_stmt><decl><type>struct <name>Z</name></type> <name>X</name></decl>;</decl_stmt></extern>
```


#### Extern Variable with Linkage Specifier

```C
extern "C" int X;
```
```XML
<extern>extern <lit:literal type="string">&quot;C&quot;</lit:literal> <decl_stmt><decl><type><name>int</name></type> <name>X</name></decl>;</decl_stmt></extern>
```


## static
### Element
* **&lt;specifier&gt;** 


### Examples

#### 1

```C
static int x;
```
```XML
<decl_stmt><decl><type><specifier>static</specifier> <name>int</name></type> <name>x</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all static functions declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl/src:type/src:specifier [.='static']`<br/>
<br/>
#### 2

```C
static void foo();
```
```XML
<function_decl><type><specifier>static</specifier> <name>void</name></type> <name>foo</name><parameter_list>()</parameter_list>;</function_decl>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all static variables declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl/src:type/src:specifier [.='static']`<br/>
<br/>
## inline
### Element
* **&lt;specifier&gt;** 


### Example

```C
inline void swapValues(int* a,int* b);
```
```XML
<function_decl><type><specifier>inline</specifier> <name>void</name></type> <name>swapValues</name><parameter_list>(<param><decl><type><name>int</name><type:modifier>*</type:modifier></type> <name>a</name></decl></param>,<param><decl><type><name>int</name><type:modifier>*</type:modifier></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all inline function declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl/src:type/src:specifier [.='inline']`<br/>
<br/>
## variable declaration statement
### Element
* **&lt;decl_stmt&gt;** 


### Examples

#### A Trivial Example

```C
int x;
```
```XML
<decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all declaration statements.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl_stmt`<br/>
<br/>
#### struct, union and enum declarations

```C
struct X y;
union X y;
enum X y;
```
```XML
<decl_stmt><decl><type>struct <name>X</name></type> <name>y</name></decl>;</decl_stmt>
<decl_stmt><decl><type>union <name>X</name></type> <name>y</name></decl>;</decl_stmt>
<decl_stmt><decl><type>enum <name>X</name></type> <name>y</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all variable declaration statements with the union qualifier.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl_stmt/src:decl[src:type/text()[contains(., 'union')]]`<br/>
<br/>
## variable declaration
### Element
* **&lt;decl&gt;** 

### SubElement
* **&lt;init&gt;** 
     * Only used when variable is initialized at declaration. See initialization examples for more details.


### Examples

#### A Trivial Example

```C
int x;
```
```XML
<decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all variable declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all variable declarations which also initialize the variable.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl[src:init]`<br/>
<br/>
#### struct, union and enum declarations

```C
struct X y;
union X y;
enum X y;
```
```XML
<decl_stmt><decl><type>struct <name>X</name></type> <name>y</name></decl>;</decl_stmt>
<decl_stmt><decl><type>union <name>X</name></type> <name>y</name></decl>;</decl_stmt>
<decl_stmt><decl><type>enum <name>X</name></type> <name>y</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all variable declarations with the struct qualifier.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl[src:type/text()[contains(., 'struct')]]`<br/>
<br/>
## function declaration
### Element
* **&lt;function_decl&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;parameter_list&gt;** 
* **&lt;param&gt;** 


### Examples

#### 1

```C
struct STUDENT sortstu( STUDENT a, STUDENT b );
void print(STUDENT a);
```
```XML
<function_decl><type>struct <name>STUDENT</name></type> <name>sortstu</name><parameter_list>( <param><decl><type><name>STUDENT</name></type> <name>a</name></decl></param>, <param><decl><type><name>STUDENT</name></type> <name>b</name></decl></param> )</parameter_list>;</function_decl>
<function_decl><type><name>void</name></type> <name>print</name><parameter_list>(<param><decl><type><name>STUDENT</name></type> <name>a</name></decl></param>)</parameter_list>;</function_decl>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all function declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all return types from all function declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl/src:type`<br/>
<br/>
#### 2

```C
void foo (int a[]);
```
```XML
<function_decl><type><name>void</name></type> <name>foo</name> <parameter_list>(<param><decl><type><name>int</name></type> <name><name>a</name><index>[]</index></name></decl></param>)</parameter_list>;</function_decl>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all parameters from all function declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl/src:parameter_list/src:param`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all function declarations with only two parameters.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl[src:parameter_list[count(src:param) = 2]`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find the names of all function declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl/src:name`<br/>
<br/>
#### 3

```C
void foo (int* a);
```
```XML
<function_decl><type><name>void</name></type> <name>foo</name> <parameter_list>(<param><decl><type><name>int</name><type:modifier>*</type:modifier></type> <name>a</name></decl></param>)</parameter_list>;</function_decl>
```


#### 4

```C
int add_multiple_values (int number, ...);
```
```XML
<function_decl><type><name>int</name></type> <name>add_multiple_values</name> <parameter_list>(<param><decl><type><name>int</name></type> <name>number</name></decl></param>, <param><decl><type><type:modifier>...</type:modifier></type></decl></param>)</parameter_list>;</function_decl>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all function declarations that has variadic arguments.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl[src:parameter_list/src:param/src:decl/src:type/type:modifier[.='...']]`<br/>
<br/>
## function definition
### Element
* **&lt;function&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;parameter_list&gt;** 
* **&lt;param&gt;** 


### Examples

#### 1

```C
struct STUDENT sortstu( STUDENT a, STUDENT b ) {
    struct STUDENT x;
    return x;
}
```
```XML
<function><type>struct <name>STUDENT</name></type> <name>sortstu</name><parameter_list>( <param><decl><type><name>STUDENT</name></type> <name>a</name></decl></param>, <param><decl><type><name>STUDENT</name></type> <name>b</name></decl></param> )</parameter_list> <block>{
    <decl_stmt><decl><type>struct <name>STUDENT</name></type> <name>x</name></decl>;</decl_stmt>
    <return>return <expr><name>x</name></expr>;</return>
}</block></function>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all function definitions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all return types from all function definitions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function/src:type`<br/>
<br/>
#### 2

```C
void print(STUDENT a) { }
```
```XML
<function><type><name>void</name></type> <name>print</name><parameter_list>(<param><decl><type><name>STUDENT</name></type> <name>a</name></decl></param>)</parameter_list> <block>{ }</block></function>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all parameters from all function definitions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function/src:parameter_list/src:param`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all function definitions with only two parameters.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function[src:parameter_list[count(src:param) = 2]`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find the names of all function definitions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function/src:name`<br/>
<br/>
#### 3

```C
void foo (int a[]) { } 
```
```XML
<function><type><name>void</name></type> <name>foo</name> <parameter_list>(<param><decl><type><name>int</name></type> <name><name>a</name><index>[]</index></name></decl></param>)</parameter_list> <block>{ }</block></function> 
```


#### 4

```C
void foo (int* a) { }
```
```XML
<function><type><name>void</name></type> <name>foo</name> <parameter_list>(<param><decl><type><name>int</name><type:modifier>*</type:modifier></type> <name>a</name></decl></param>)</parameter_list> <block>{ }</block></function>
```


#### 5

```C
int add_multiple_values (int number, ...) { }
```
```XML
<function><type><name>int</name></type> <name>add_multiple_values</name> <parameter_list>(<param><decl><type><name>int</name></type> <name>number</name></decl></param>, <param><decl><type><type:modifier>...</type:modifier></type></decl></param>)</parameter_list> <block>{ }</block></function>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all function definition that has variadic arguments.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function[src:parameter_list/src:param/src:decl/src:type/type:modifier[.='...']]`<br/>
<br/>
#### 6

```C
int func(a, b, c)
   int a;
   int b;
   int c;
{ }
```
```XML
<function><type><name>int</name></type> <name>func</name><parameter_list>(<param><decl><type><name>a</name></type></decl></param>, <param><decl><type><name>b</name></type></decl></param>, <param><decl><type><name>c</name></type></decl></param>)</parameter_list>
   <decl_stmt><decl><type><name>int</name></type> <name>a</name></decl>;</decl_stmt>
   <decl_stmt><decl><type><name>int</name></type> <name>b</name></decl>;</decl_stmt>
   <decl_stmt><decl><type><name>int</name></type> <name>c</name></decl>;</decl_stmt>
<block>{ }</block></function>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all K & R function definitions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function[src:decl_stmt]`<br/>
<br/>
## pointer declaration
### Element
* **&lt;type:modifier&gt;** 
     * **Note**: This mark up is disabled by default and is enabled using the following command line argument/s with srcML: 
        * --modifier


### Examples

#### 1

```C
char *message;
```
```XML
<decl_stmt><decl><type><name>char</name> <type:modifier>*</type:modifier></type><name>message</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of the declarations of pointers.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl[type:modifier[.='*']]`<br/>
<br/>
#### 2

```C
int *pointers[10];
```
```XML
<decl_stmt><decl><type><name>int</name> <type:modifier>*</type:modifier></type><name><name>pointers</name><index>[<expr><lit:literal type="number">10</lit:literal></expr>]</index></name></decl>;</decl_stmt>
```


#### 3

```C
int (*pointer)[10];
```
```XML
<expr_stmt><expr><call><name>int</name> <argument_list>(<argument><expr><op:operator>*</op:operator><name>pointer</name></expr></argument>)</argument_list></call><index>[<expr><lit:literal type="number">10</lit:literal></expr>]</index></expr>;</expr_stmt>
```


#### 4

```C
int const *x;
```
```XML
<decl_stmt><decl><type><name>int</name> <specifier>const</specifier> <type:modifier>*</type:modifier></type><name>x</name></decl>;</decl_stmt>
```


#### 5

```C
int(*X)(void);
int(*X)(int, float);
int(*X)(char const*,...);
```
```XML
<function_decl><type><name>int</name></type>(<type:modifier>*</type:modifier><name>X</name>)<parameter_list>(<param><decl><type><name>void</name></type></decl></param>)</parameter_list>;</function_decl>
<function_decl><type><name>int</name></type>(<type:modifier>*</type:modifier><name>X</name>)<parameter_list>(<param><decl><type><name>int</name></type></decl></param>, <param><decl><type><name>float</name></type></decl></param>)</parameter_list>;</function_decl>
<function_decl><type><name>int</name></type>(<type:modifier>*</type:modifier><name>X</name>)<parameter_list>(<param><decl><type><name>char</name> <specifier>const</specifier><type:modifier>*</type:modifier></type></decl></param>,<param><decl><type><type:modifier>...</type:modifier></type></decl></param>)</parameter_list>;</function_decl>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all function pointers.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:function_decl[text()[contains(., ')')]]`<br/>
<br/>
#### 6

```C
int volatile *const z = &some_object;
```
```XML
<decl_stmt><decl><type><name>int</name> <specifier>volatile</specifier> <type:modifier>*</type:modifier><specifier>const</specifier></type> <name>z</name> <init>= <expr><op:operator>&amp;</op:operator><name>some_object</name></expr></init></decl>;</decl_stmt>
```


#### 7

```C
int *const volatile w = &some_object;
```
```XML
<decl_stmt><decl><type><name>int</name> <type:modifier>*</type:modifier><specifier>const</specifier> <specifier>volatile</specifier></type> <name>w</name> <init>= <expr><op:operator>&amp;</op:operator><name>some_object</name></expr></init></decl>;</decl_stmt>
```


## typedef
### Element
* **&lt;typedef&gt;** 

### SubElements
* **&lt;type&gt;** 
* **&lt;name&gt;** 
* **&lt;function_decl&gt;** 


### Examples

#### 1

```C
typedef char C;
typedef unsigned int WORD;
typedef char * pChar;
typedef char field [50]; 
typedef char field, *field2; 
```
```XML
<typedef>typedef <type><name>char</name></type> <name>C</name>;</typedef>
<typedef>typedef <type><name>unsigned</name> <name>int</name></type> <name>WORD</name>;</typedef>
<typedef>typedef <type><name>char</name> <type:modifier>*</type:modifier></type> <name>pChar</name>;</typedef>
<typedef>typedef <type><name>char</name></type> <name><name>field</name> <index>[<expr><lit:literal type="number">50</lit:literal></expr>]</index></name>;</typedef> 
<typedef>typedef <type><name>char</name></type> <name>field</name><op:operator>,</op:operator> <type:modifier>*</type:modifier><name>field2</name>;</typedef> 
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all typedefs.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:typedef`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all types within typedefs.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:typedef/src:type`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all names for all typedefs.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:typedef/src:name`<br/>
<br/>
#### 2

```C
typedef struct club 
{
    char name[30];
    int size, year;
} GROUP;
```
```XML
<typedef>typedef <type><struct>struct <name>club</name> 
<block>{
    <decl_stmt><decl><type><name>char</name></type> <name><name>name</name><index>[<expr><lit:literal type="number">30</lit:literal></expr>]</index></name></decl>;</decl_stmt>
    <decl_stmt><decl><type><name>int</name></type> <name>size</name></decl><op:operator>,</op:operator> <name>year</name>;</decl_stmt>
}</block></struct></type> <name>GROUP</name>;</typedef>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all typedefs that are used to define a struct.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:typedef[src:type/src:struct]`<br/>
<br/>
#### 3

```C
typedef void DRAWF( int, int );
```
```XML
<typedef>typedef <function_decl><type><name>void</name></type> <name>DRAWF</name><parameter_list>( <param><decl><type><name>int</name></type></decl></param>, <param><decl><type><name>int</name></type></decl></param> )</parameter_list>;</function_decl></typedef>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all typedefs that declare a function type.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:typedef[src:function_decl]`<br/>
<br/>
#### 4

```C
typedef void (*DRAWF)( int, int );
```
```XML
<typedef>typedef <function_decl><type><name>void</name></type> (<type:modifier>*</type:modifier><name>DRAWF</name>)<parameter_list>( <param><decl><type><name>int</name></type></decl></param>, <param><decl><type><name>int</name></type></decl></param> )</parameter_list>;</function_decl></typedef>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all typedefs of a function pointer.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:typedef[src:function_decl[text()[contains(., ')')]]]`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all names from all typedefs of function pointers.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:typedef/src:function_decl[text()[contains(., ')')]]/src:name`<br/>
<br/>
## array declaration
### Element
* **&lt;decl&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;index&gt;** 


### Examples

#### 1

```C
char A[2];
```
```XML
<decl_stmt><decl><type><name>char</name></type> <name><name>A</name><index>[<expr><lit:literal type="number">2</lit:literal></expr>]</index></name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all array declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl[src:name/src:name/src:index]]`<br/>
<br/>
#### 2

```C
float matrix[10][15];
```
```XML
<decl_stmt><decl><type><name>float</name></type> <name><name>matrix</name><index>[<expr><lit:literal type="number">10</lit:literal></expr>]</index><index>[<expr><lit:literal type="number">15</lit:literal></expr>]</index></name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all array declarations where the number of elements is less then 50.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl[src:name/src:name/src:index/lit:literal[number(text()) < 50]]`<br/>
<br/>
#### 3

```C
struct {
    float x, y;
} complex[100];
```
```XML
<struct>struct <block>{
    <decl_stmt><decl><type><name>float</name></type> <name>x</name></decl><op:operator>,</op:operator> <decl><type ref="prev"/><name>y</name></decl>;</decl_stmt>
}</block> <decl><name><name>complex</name><index>[<expr><lit:literal type="number">100</lit:literal></expr>]</index></name></decl>;</struct>
```


#### 4

```C
extern char *name[];
```
```XML
<decl_stmt><decl><type><specifier>extern</specifier> <name>char</name> <type:modifier>*</type:modifier></type><name><name>name</name><index>[]</index></name></decl>;</decl_stmt>
```


## array initialization
### Element
* **&lt;init&gt;** 

### SubElement
* **&lt;block&gt;** 


### Examples

#### 1

```C
int myArray[10] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
```
```XML
<decl_stmt><decl><type><name>int</name></type> <name><name>myArray</name><index>[<expr><lit:literal type="number">10</lit:literal></expr>]</index></name> <init>= <expr><block>{ <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr>, <expr><lit:literal type="number">5</lit:literal></expr> }</block></expr></init></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all array declarations which initialize the array at the same time.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl[src:name/src:name/src:index]][src:init]`<br/>
<br/>
#### 2

```C
int a[6] = { [4] = 29, [2] = 15 };
```
```XML
<decl_stmt><decl><type><name>int</name></type> <name><name>a</name><index>[<expr><lit:literal type="number">6</lit:literal></expr>]</index></name> <init>= <expr><block>{ <expr><index>[<expr><lit:literal type="number">4</lit:literal></expr>]</index> <op:operator>=</op:operator> <lit:literal type="number">29</lit:literal></expr>, <expr><index>[<expr><lit:literal type="number">2</lit:literal></expr>]</index> <op:operator>=</op:operator> <lit:literal type="number">15</lit:literal></expr> }</block></expr></init></decl>;</decl_stmt>
```


## bitfield
### Element
* **&lt;range&gt;** 

### SubElements
* **&lt;expr&gt;** 
* **&lt;lit:literal&gt;** 
     * **Note**: This mark up is disabled by default and is enabled using the following command line argument/s with srcML: 
        * --literal


### Example

```C
struct mybitfields
{
    int a : 5;
} test;
```
```XML
<struct>struct <name>mybitfields</name>
<block>{
    <decl_stmt><decl><type><name>int</name></type> <name>a</name> <range>: <expr><lit:literal type="number">5</lit:literal></expr></range></decl>;</decl_stmt>
}</block> <decl><name>test</name></decl>;</struct>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all bitfield declarations.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl[src:range]`<br/>
<br/>
## struct declaration
### Element
* **&lt;struct_decl&gt;** 

### SubElement
* **&lt;name&gt;** 


### Example

#### forward declaration

```C
struct employee;
```
```XML
<struct_decl>struct <name>employee</name>;</struct_decl>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of the forward declarations for all structs.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:struct_decl`<br/>
<br/>
## struct definition
### Element
* **&lt;struct&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;block&gt;** 


### Examples

#### trivial declaration

```C
struct a {
    int x;
    double y;
    float z;
 };
```
```XML
<struct>struct <name>a</name> <block>{
    <decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt>
    <decl_stmt><decl><type><name>double</name></type> <name>y</name></decl>;</decl_stmt>
    <decl_stmt><decl><type><name>float</name></type> <name>z</name></decl>;</decl_stmt>
 }</block>;</struct>
```

####XPath Query Examples

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find the definition of all structs.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:struct`<br/>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find the members from all structs.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:struct/src:block/src:decl_stmt`<br/>
<br/>
#### variable of an anonymous struct type

```C
struct
{
    int x;
    int y;
} mystruct;
```
```XML
<struct>struct
<block>{
    <decl_stmt><decl><type><name>int</name></type> <name>x</name></decl>;</decl_stmt>
    <decl_stmt><decl><type><name>int</name></type> <name>y</name></decl>;</decl_stmt>
}</block> <decl><name>mystruct</name></decl>;</struct>
```


#### declaring a struct and instance

```C
struct employee
{
    char name[20];
    int id;
    long class;
} temp;
```
```XML
<struct>struct <name>employee</name>
<block>{
    <decl_stmt><decl><type><name>char</name></type> <name><name>name</name><index>[<expr><lit:literal type="number">20</lit:literal></expr>]</index></name></decl>;</decl_stmt>
    <decl_stmt><decl><type><name>int</name></type> <name>id</name></decl>;</decl_stmt>
    <decl_stmt><decl><type><name>long</name></type> <name>class</name></decl>;</decl_stmt>
}</block> <decl><name>temp</name></decl>;</struct>
```


## struct initialization
### Element
* **&lt;decl_stmt&gt;** 

### SubElements
* **&lt;decl&gt;** 
* **&lt;init&gt;** 
* **&lt;block&gt;** 


### Examples

#### 1

```C
MY_TYPE a = { false, 234, 1.234 };
```
```XML
<decl_stmt><decl><type><name>MY_TYPE</name></type> <name>a</name> <init>= <expr><block>{ <expr><name>false</name></expr>, <expr><lit:literal type="number">234</lit:literal></expr>, <expr><lit:literal type="number">1.234</lit:literal></expr> }</block></expr></init></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of declaration of structs which also initialize it.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:decl[src:init/src:block]`<br/>
<br/>
#### 2

```C
MY_TYPE b = (MY_TYPE) { false, 234, 1.234 };
```
```XML
<decl_stmt><decl><type><name>MY_TYPE</name></type> <name>b</name> <init>= <expr><op:operator>(</op:operator><name>MY_TYPE</name><op:operator>)</op:operator> <block>{ <expr><name>false</name></expr>, <expr><lit:literal type="number">234</lit:literal></expr>, <expr><lit:literal type="number">1.234</lit:literal></expr> }</block></expr></init></decl>;</decl_stmt>
```


#### 3

```C
MY_TYPE c = { .flag = true, .value = 123, .stuff = 0.456 };
```
```XML
<decl_stmt><decl><type><name>MY_TYPE</name></type> <name>c</name> <init>= <expr><block>{ <expr><op:operator>.</op:operator><name>flag</name> <op:operator>=</op:operator> <name>true</name></expr>, <expr><op:operator>.</op:operator><name>value</name> <op:operator>=</op:operator> <lit:literal type="number">123</lit:literal></expr>, <expr><op:operator>.</op:operator><name>stuff</name> <op:operator>=</op:operator> <lit:literal type="number">0.456</lit:literal></expr> }</block></expr></init></decl>;</decl_stmt>
```


## enum definition
### Element
* **&lt;enum&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;block&gt;** 


### Examples

#### 1

```C
enum DAY {
    sunday = 0,
    monday,
    tuesday,
    wednesday,
    thursday,
    friday,
    saturday,
};
```
```XML
<enum>enum <name>DAY</name> <block>{
    <decl><name>sunday</name> <init>= <expr><lit:literal type="number">0</lit:literal></expr></init></decl><op:operator>,</op:operator>
    <decl><name>monday</name></decl><op:operator>,</op:operator>
    <decl><name>tuesday</name></decl><op:operator>,</op:operator>
    <decl><name>wednesday</name></decl><op:operator>,</op:operator>
    <decl><name>thursday</name></decl><op:operator>,</op:operator>
    <decl><name>friday</name></decl><op:operator>,</op:operator>
    <decl><name>saturday</name></decl><op:operator>,</op:operator>
}</block>;</enum>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all enum definitions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:enum`<br/>
<br/>
#### 2

```C
enum DAY;
```
```XML
<enum>enum <name>DAY</name>;</enum>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find the names of all members of all enums.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:enum/src:decl/src:name`<br/>
<br/>
#### 3

```C
enum { yes, no } response;
```
```XML
<enum>enum <block>{ <decl><name>yes</name></decl><op:operator>,</op:operator> <decl><name>no</name></decl> }</block> <decl><name>response</name></decl>;</enum>
```


## union declaration
### Element
* **&lt;union_decl&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;block&gt;** 


### Example

```C
union X;
```
```XML
<union_decl>union <name>X</name>;</union_decl>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all of the forward declarations for all unions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:union_decl`<br/>
<br/>
## union definition
### Element
* **&lt;union&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;block&gt;** 


### Example

```C
union sign
{
    int svar;
    unsigned uvar;
} number;
```
```XML
<union>union <name>sign</name>
<block>{
    <decl_stmt><decl><type><name>int</name></type> <name>svar</name></decl>;</decl_stmt>
    <decl_stmt><decl><type><name>unsigned</name></type> <name>uvar</name></decl>;</decl_stmt>
}</block> <decl><name>number</name></decl>;</union>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all definitions of all unions.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:union`<br/>
<br/>
## Function Call
### Element
* **&lt;call&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;argument_list&gt;** 
* **&lt;argument&gt;** 


### Example

```C
foo(x, y, z);
```
```XML
<expr_stmt><expr><call><name>foo</name><argument_list>(<argument><expr><name>x</name></expr></argument>, <argument><expr><name>y</name></expr></argument>, <argument><expr><name>z</name></expr></argument>)</argument_list></call></expr>;</expr_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all function calls.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:call`<br/>
<br/>
## array indexing
### Element
* **&lt;name&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;index&gt;** 


### Example

```C
a = x[5];
val = matrix[x][y];
```
```XML
<expr_stmt><expr><name>a</name> <op:operator>=</op:operator> <name><name>x</name><index>[<expr><lit:literal type="number">5</lit:literal></expr>]</index></name></expr>;</expr_stmt>
<expr_stmt><expr><name>val</name> <op:operator>=</op:operator> <name><name>matrix</name><index>[<expr><name>x</name></expr>]</index><index>[<expr><name>y</name></expr>]</index></name></expr>;</expr_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all array indexings.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:expr/src:name[src:name/src:index/node()]`<br/>
<br/>
## dereference member access
### Element
* **&lt;expr&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;op:operator&gt;** 
     * **Note**: This mark up is disabled by default and is enabled using the following command line argument/s with srcML: 
        * --operator


### Example

```C
 t->tm_sec;
```
```XML
 <expr_stmt><expr><name><name>t</name><op:operator>-&gt;</op:operator><name>tm_sec</name></name></expr>;</expr_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of the `->` operator.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//op:operator[.='->']`<br/>
<br/>
## member access
### Element
* **&lt;expr&gt;** 

### SubElements
* **&lt;name&gt;** 
* **&lt;op:operator&gt;** 
     * **Note**: This mark up is disabled by default and is enabled using the following command line argument/s with srcML: 
        * --operator


### Example

```C
t.tm_sec;
```
```XML
<expr_stmt><expr><name><name>t</name><op:operator>.</op:operator><name>tm_sec</name></name></expr>;</expr_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of the `.` operator.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//op:operator[.='.']`<br/>
<br/>
## cast
### Element
* **&lt;op:operator&gt;** 
     * **Note**: This mark up is disabled by default and is enabled using the following command line argument/s with srcML: 
        * --operator

### SubElement
* **&lt;name&gt;** 


### Example

```C
double x = 0.0;
int y = (int)x;

```
```XML
<decl_stmt><decl><type><name>double</name></type> <name>x</name> <init>= <expr><lit:literal type="number">0.0</lit:literal></expr></init></decl>;</decl_stmt>
<decl_stmt><decl><type><name>int</name></type> <name>y</name> <init>= <expr><op:operator>(</op:operator><name>int</name><op:operator>)</op:operator><name>x</name></expr></init></decl>;</decl_stmt>

```


## sizeof
### Element
* **&lt;sizeof&gt;** 

### SubElements
* **&lt;argument_list&gt;** 
* **&lt;argument&gt;** 


### Example

```C
sizeof(struct type);
```
```XML
<expr_stmt><expr><sizeof>sizeof<argument_list>(<argument>struct <expr><name>type</name></expr></argument>)</argument_list></sizeof></expr>;</expr_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of sizeof.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:sizeof`<br/>
<br/>
## _Alignas
### Element
* **&lt;alignas&gt;** 

### SubElements
* **&lt;argument_list&gt;** 
* **&lt;argument&gt;** 


### Examples

#### 1

```C
_Alignas(struct X) struct X y;
```
```XML
<decl_stmt><decl><type><alignas>_Alignas<argument_list>(</argument_list></alignas>struct <name>X</name></type></decl>) struct X y;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of _Alignas.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:alignas`<br/>
<br/>
#### 2

```C
_Alignas(64) int y[4];
```
```XML
<decl_stmt><decl><type><alignas>_Alignas<argument_list>(<argument><expr><lit:literal type="number">64</lit:literal></expr></argument>)</argument_list></alignas> <name>int</name></type> <name><name>y</name><index>[<expr><lit:literal type="number">4</lit:literal></expr>]</index></name></decl>;</decl_stmt>
```


## _Alignof
### Element
* **&lt;alignof&gt;** 

### SubElements
* **&lt;argument_list&gt;** 
* **&lt;argument&gt;** 


### Example

```C
_Alignof(union type);
```
```XML
<expr_stmt><expr><alignof>_Alignof<argument_list>(<argument><expr>union <name>type</name></expr></argument>)</argument_list></alignof></expr>;</expr_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of _Alignof.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:alignof`<br/>
<br/>
## _Atomic
### Element
* **&lt;atomic&gt;** 

### SubElements
* **&lt;argument_list&gt;** 
* **&lt;argument&gt;** 


### Examples

#### 1

```C
_Atomic union X t;
```
```XML
_Atomic <decl_stmt><decl><type>union <name>X</name></type> <name>t</name></decl>;</decl_stmt>
```


#### 2

```C
_Atomic int t;
```
```XML
<decl_stmt><decl><type><atomic>_Atomic</atomic> <name>int</name></type> <name>t</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of _Atomic.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:atomic`<br/>
<br/>
#### 3

```C
_Atomic(int) t;
```
```XML
<decl_stmt><decl><type><atomic>_Atomic<argument_list>(<argument><expr><name>int</name></expr></argument>)</argument_list></atomic></type> <name>t</name></decl>;</decl_stmt>
```


## _Generic

The only use case for this keyword is within a macro, in which case srcML will not mark it up.
### Element
* **&lt;generic_selection&gt;** 

### SubElements
* **&lt;selector&gt;** 
* **&lt;association_list&gt;** 
* **&lt;association&gt;** 


### Example

```C
_Generic ( AnyExpression , int:5, double:5.0, default:0);
```
```XML
<expr_stmt><expr><generic_selection>_Generic ( <selector><expr><name>AnyExpression</name></expr></selector> <op:operator>,</op:operator> <association_list><association><type><name>int</name></type>:<expr><lit:literal type="number">5</lit:literal></expr></association><op:operator>,</op:operator> <association><type><name>double</name></type>:<expr><lit:literal type="number">5.0</lit:literal></expr></association><op:operator>,</op:operator> <association><type>default</type>:<expr><lit:literal type="number">0</lit:literal></expr></association></association_list>)</generic_selection></expr>;</expr_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of _Generic.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:generic_selection`<br/>
<br/>
## _Noreturn
### Element
* **&lt;specifier&gt;** 


### Example

```C
_Noreturn void die(const char *fmt, ...) { exit(EXIT_FAILURE); }
```
```XML
<function><type><specifier>_Noreturn</specifier> <name>void</name></type> <name>die</name><parameter_list>(<param><decl><type><specifier>const</specifier> <name>char</name> <type:modifier>*</type:modifier></type><name>fmt</name></decl></param>, <param><decl><type><type:modifier>...</type:modifier></type></decl></param>)</parameter_list> <block>{ <expr_stmt><expr><call><name>exit</name><argument_list>(<argument><expr><name>EXIT_FAILURE</name></expr></argument>)</argument_list></call></expr>;</expr_stmt> }</block></function>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of _Noreturn.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:specifier[.='_Noreturn']`<br/>
<br/>
## _Static_assert
### Element
* **&lt;assert type="static"&gt;** 

### SubElements
* **&lt;argument_list&gt;** 
* **&lt;argument&gt;** 


### Example

```C
_Static_assert(DEBUG_LEVEL > 5, "Message");
```
```XML
<assert type="static">_Static_assert<argument_list>(<argument><expr><name>DEBUG_LEVEL</name> <op:operator>&gt;</op:operator> <lit:literal type="number">5</lit:literal></expr></argument>, <argument><expr><lit:literal type="string">&quot;Message&quot;</lit:literal></expr></argument>)</argument_list>;</assert>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of _Static_assert.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:assert[@type ='static']`<br/>
<br/>
## _Thread_local
### Element
* **&lt;specifier&gt;** 


### Example

```C
_Thread_local int x;
```
```XML
<decl_stmt><decl><type><specifier>_Thread_local</specifier> <name>int</name></type> <name>x</name></decl>;</decl_stmt>
```

####XPath Query Example

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Description:**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Find all uses of _Thread_local.<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**XPath**<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`//src:specifier[.='_Thread_local']`<br/>
<br/>

## Operators
Name | Operator | srcML
--- | --- | ---
Pre Increment | `++` | `<op:operator>++</op:operator>`
Post Increment | `++` | `<op:operator>++</op:operator>`
Pre Decrement | `--` | `<op:operator>--</op:operator>`
Post Decrement | `--` | `<op:operator>--</op:operator>`
Address Of | `&` | `<op:operator>&amp;</op:operator>`
Dereference | `*` | `<op:operator>*</op:operator>`
Unary Minus | `-` | `<op:operator>-</op:operator>`
Bitwise Not | `~` | `<op:operator>~</op:operator>`
Logical Not | `!` | `<op:operator>!</op:operator>`
Multiplication | `*` | `<op:operator>*</op:operator>`
Division | `/` | `<op:operator>/</op:operator>`
Modulus | `%` | `<op:operator>%</op:operator>`
Addition | `+` | `<op:operator>+</op:operator>`
Subtraction | `-` | `<op:operator>-</op:operator>`
Left Shift | `<<` | `<op:operator>&lt;&lt;</op:operator>`
Right Shift | `>>` | `<op:operator>&gt;&gt;</op:operator>`
Less Then | `<` | `<op:operator>&lt;</op:operator>`
Greater Then | `>` | `<op:operator>&gt;</op:operator>`
Less Then or Equal To | `<=` | `<op:operator>&lt;=</op:operator>`
Greater Then or Equal To | `>=` | `<op:operator>&gt;=</op:operator>`
Equals | `==` | `<op:operator>==</op:operator>`
Not Equal | `!=` | `<op:operator>!=</op:operator>`
Bitwise And | `&` | `<op:operator>&amp;</op:operator>`
Bitwise XOr | `^` | `<op:operator>^</op:operator>`
Bitwise Or | <code>&#124;</code> | <code>&lt;op:operator&gt;&#124;&lt;/op:operator&gt;</code>
Logical And | `&&` | `<op:operator>&amp;&amp;</op:operator>`
Logical Or | <code>&#124;&#124;</code> | <code>&lt;op:operator&gt;&#124;&#124;&lt;/op:operator&gt;</code>
Assignment | `=` | `<op:operator>=</op:operator>`
Multiply Assign | `*=` | `<op:operator>*=</op:operator>`
Divide Assign | `/=` | `<op:operator>/=</op:operator>`
Modulus Assign | `%=` | `<op:operator>%=</op:operator>`
Add Assign | `+=` | `<op:operator>+=</op:operator>`
Subtract Assign | `-=` | `<op:operator>-=</op:operator>`
Left Shift Assign | `<<=` | `<op:operator>&lt;&lt;=</op:operator>`
Right Shift Assign | `>>=` | `<op:operator>&gt;&gt;=</op:operator>`
Bitwise And Assign | `&=` | `<op:operator>&amp;=</op:operator>`
Bitwise XOr Assign | `^=` | `<op:operator>^=</op:operator>`
Bitwise Or Assign | <code>&#124;=</code> | <code>&lt;op:operator&gt;&#124;=&lt;/op:operator&gt;</code>
Comma | `,` | `<op:operator>,</op:operator>`
Conditional | `?  :` | `<op:operator>?</op:operator>`<br/>`<op:operator>:</op:operator>`
Dot | `.` | `<op:operator>.</op:operator>`
Arrow | `->` | `<op:operator>-&gt;</op:operator>`

**NOTE:** Operator mark up within srcML is not enabled by default and srcML must be run with the --operator command line option to enable it.