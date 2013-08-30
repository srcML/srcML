/* srcML XPath query and XSLT transform functions */
int srcml_add_transform_xpath(srcml_archive* archive, const char* xpath_string) {

  transform tran = { SRCML_XPATH, xpath_string };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

int srcml_add_transform_xslt(srcml_archive* archive, const char* xslt_filename) {

  transform tran = { SRCML_XSLT, xslt_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

int srcml_add_transform_relaxng(srcml_archive* archive, const char* relaxng_filename) {

  transform tran = { SRCML_RELAXNG, relaxng_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}


// TODO finish.  what happends to intermediate results?
int srcml_apply_transforms(srcml_archive* iarchive, srcml_archive* oarchive) {

  iarchive->transformations.clear();

  return SRCML_STATUS_OK;

}
