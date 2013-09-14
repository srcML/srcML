/*

  Test cases for srcml_apply_transforms
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  const std::string srcml_a = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n";
  const std::string srcml_b = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n";
  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";
  const std::string srcml_a_after = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" filename=\"a.cpp\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";
  const std::string srcml_b_after = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" dir=\"test\" filename=\"project\" version=\"1\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";
  const std::string srcml_full = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

  /* 
     srcml_apply_transforms
   */

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml.c_str(), srcml.size());
    srcml_append_transform_xpath(iarchive, "//src:unit");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    //assert(s == srcml);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
    srcml_append_transform_xpath(iarchive, "//src:unit");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    //assert(s == srcml_full);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
    srcml_append_transform_xpath(iarchive, "//src:unit");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    //assert(s == srcml_a_after);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
    srcml_append_transform_xpath(iarchive, "//src:unit");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    //assert(s == srcml_b_after);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml.c_str(), srcml.size());
    srcml_append_transform_xslt(iarchive, "copy.xsl");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    assert(s == srcml);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
    srcml_append_transform_xslt(iarchive, "copy.xsl");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    assert(s == srcml_full);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
    srcml_append_transform_xslt(iarchive, "copy.xsl");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    assert(s == srcml_a_after);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
    srcml_append_transform_xslt(iarchive, "copy.xsl");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    assert(s == srcml_b_after);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml.c_str(), srcml.size());
    srcml_append_transform_relaxng(iarchive, "schema.rng");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    assert(s == srcml);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_full.c_str(), srcml_full.size());
    srcml_append_transform_relaxng(iarchive, "schema.rng");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    assert(s == srcml_full);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_a.c_str(), srcml_a.size());
    srcml_append_transform_relaxng(iarchive, "schema.rng");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    assert(s == srcml_a_after);
  }

  {
    char * s;
    srcml_archive * iarchive = srcml_create_archive();
    srcml_read_open_memory(iarchive, srcml_b.c_str(), srcml_b.size());
    srcml_append_transform_relaxng(iarchive, "schema.rng");
    srcml_archive * oarchive = srcml_clone_archive(iarchive);
    srcml_write_open_memory(oarchive, &s);

    srcml_apply_transforms(iarchive, oarchive);

    srcml_close_archive(oarchive);
    srcml_free_archive(oarchive);  
    srcml_close_archive(iarchive);
    srcml_free_archive(iarchive);  
    assert(s == srcml_b_after);
  }

  return 0;

}
