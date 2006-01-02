<?xml version="1.0" encoding="UTF-8" ?>

<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="actual_params">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="param" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="asm">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="block">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="public" minOccurs="1" maxOccurs="1" />
        <xs:element ref="private" minOccurs="1" maxOccurs="1" />
        <xs:element ref="protected" minOccurs="1" maxOccurs="1" />
        <xs:element ref="case" minOccurs="1" maxOccurs="1" />
        <xs:element ref="default" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="break">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="call">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="actual_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="case">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="catch">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="param" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="class">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="super" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="class_decl">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="super" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="comment">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
      <xs:attribute name="type" use="required">
        <xs:simpleType>
          <xs:restriction base="xs:NMTOKEN">
            <xs:enumeration value="block" />
            <xs:enumeration value="line" />
          </xs:restriction>
        </xs:simpleType>
      </xs:attribute>
    </xs:complexType>
  </xs:element>

  <xs:element name="condition">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="constructor">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="call" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="formal_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="constructor_decl">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="formal_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:define">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:directive">
    <xs:complexType mixed="true" />
  </xs:element>

  <xs:element name="cpp:elif">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="condition" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:else">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:endif">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:file">
    <xs:complexType mixed="true" />
  </xs:element>

  <xs:element name="cpp:if">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="condition" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:ifdef">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:ifndef">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:include">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:file" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:then">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="cpp:undef">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="cpp:directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="decl">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="type" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="init" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="decl_stmt">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="type" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="init" minOccurs="1" maxOccurs="1" />
        <xs:element ref="actual_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="default">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="destructor">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="formal_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="destructor_decl">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="formal_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="do">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="condition" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="else">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="enum">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="expr">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="call" minOccurs="1" maxOccurs="1" />
        <xs:element ref="type" minOccurs="1" maxOccurs="1" />
        <xs:element ref="init" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="expr_stmt">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="for">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="init" minOccurs="1" maxOccurs="1" />
        <xs:element ref="condition" minOccurs="1" maxOccurs="1" />
        <xs:element ref="incr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="formal_params">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="param" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="function">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="type" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="formal_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="param" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="function_decl">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="type" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="formal_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="param" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="goto">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="if">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="condition" minOccurs="1" maxOccurs="1" />
        <xs:element ref="then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="incr">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="init">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="label">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="name">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="param" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="namespace">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="param">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="type" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="init" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="actual_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="private">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
      <xs:attribute name="type" type="xs:string" use="optional" />
    </xs:complexType>
  </xs:element>

  <xs:element name="protected">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="public">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
      <xs:attribute name="type" type="xs:string" use="optional" />
    </xs:complexType>
  </xs:element>

  <xs:element name="return">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="specifier">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="struct">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="super" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="struct_decl">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="super" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="super">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="specifier" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="switch">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="condition" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="template">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="formal_params" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="then">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="throw">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="expr" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="try">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="type">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="typedef">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="type" minOccurs="1" maxOccurs="1" />
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="union">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="super" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="union_decl">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="super" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="unit">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="unit" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
      </xs:choice>
      <xs:attribute name="filename" type="xs:string" use="optional" />
      <xs:attribute name="dir" type="xs:string" use="optional" />
      <xs:attribute name="xmlns:cpp" type="xs:string" use="optional" />
      <xs:attribute name="xmlns" type="xs:string" use="optional" />
    </xs:complexType>
  </xs:element>

  <xs:element name="using_directive">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="name" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

  <xs:element name="while">
    <xs:complexType mixed="true">
      <xs:choice>
        <xs:element ref="condition" minOccurs="1" maxOccurs="1" />
        <xs:element ref="comment" minOccurs="1" maxOccurs="1" />
        <xs:element ref="block" minOccurs="1" maxOccurs="1" />
        <xs:element ref="expr_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="decl_stmt" minOccurs="1" maxOccurs="1" />
        <xs:element ref="if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="while" minOccurs="1" maxOccurs="1" />
        <xs:element ref="do" minOccurs="1" maxOccurs="1" />
        <xs:element ref="for" minOccurs="1" maxOccurs="1" />
        <xs:element ref="switch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="break" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function" minOccurs="1" maxOccurs="1" />
        <xs:element ref="function_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="return" minOccurs="1" maxOccurs="1" />
        <xs:element ref="try" minOccurs="1" maxOccurs="1" />
        <xs:element ref="throw" minOccurs="1" maxOccurs="1" />
        <xs:element ref="catch" minOccurs="1" maxOccurs="1" />
        <xs:element ref="template" minOccurs="1" maxOccurs="1" />
        <xs:element ref="namespace" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class" minOccurs="1" maxOccurs="1" />
        <xs:element ref="class_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct" minOccurs="1" maxOccurs="1" />
        <xs:element ref="struct_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union" minOccurs="1" maxOccurs="1" />
        <xs:element ref="union_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="constructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor" minOccurs="1" maxOccurs="1" />
        <xs:element ref="destructor_decl" minOccurs="1" maxOccurs="1" />
        <xs:element ref="typedef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="using_directive" minOccurs="1" maxOccurs="1" />
        <xs:element ref="enum" minOccurs="1" maxOccurs="1" />
        <xs:element ref="asm" minOccurs="1" maxOccurs="1" />
        <xs:element ref="label" minOccurs="1" maxOccurs="1" />
        <xs:element ref="goto" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:include" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:define" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:undef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:if" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:then" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:else" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:endif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:elif" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifdef" minOccurs="1" maxOccurs="1" />
        <xs:element ref="cpp:ifndef" minOccurs="1" maxOccurs="1" />
      </xs:choice>
    </xs:complexType>
  </xs:element>

</xs:schema>