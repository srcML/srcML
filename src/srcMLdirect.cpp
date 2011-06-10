#include <cstring>
#ifdef __GNUG__
#include <sys/stat.h>
#endif
#include "srcmlapps.hpp"
#include "srcmlns.hpp"
#include "Options.hpp"
#include "project.hpp"
#include "Language.hpp"
#include "srcMLTranslator.hpp"
#include "URIStream.hpp"
#include <getopt.h>
#include <dirent.h>
#include <algorithm>

int main(int argc, char * argv[]) {

  OPTION_TYPE options = OPTION_CPP_MARKUP_ELSE;
  const char* urisprefix[] = {

    SRCML_SRC_NS_PREFIX_DEFAULT,
    SRCML_CPP_NS_PREFIX_DEFAULT,
    SRCML_ERR_NS_PREFIX_DEFAULT,
    SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
    SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
    SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
    SRCML_EXT_POSITION_NS_PREFIX_DEFAULT,
    };

  srcMLTranslator::srcMLTranslator translator(1,
		  "UTF-8",
		  "ISO-8859-1",
		  argv[1],
		  options,
		  "",
		  argv[1],
		  "1",
		  urisprefix,
		  8
		  );

  return 0;
}
