#ifndef INCLUDED_SRCEXFUN_H
#define INCLUDED_SRCEXFUN_H

#include <libxml/xpath.h>
#include "SAX2Utilities.hpp"
#include <vector>

void setPosition(int n);

void setRootAttributes(PROPERTIES_TYPE&);

void xsltsrcMLRegister();

void xpathsrcMLRegister(xmlXPathContextPtr context);



struct xpath_ext_function {

  std::string name;
  std::string expr;
};

void xpathRegisterDefaultExtensionFunctions();

void xpathRegisterExtensionFunction(const std::string & name, const std::string & xpath);

const std::vector<xpath_ext_function> getXPathExtensionFunctions();

#endif
