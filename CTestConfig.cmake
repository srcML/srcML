set(CTEST_PROJECT_NAME "srcML")
set(CTEST_NIGHTLY_START_TIME "01:00:00 UTC")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "localhost:8080")
set(CTEST_DROP_LOCATION "/submit.php?project=srcML")
set(CTEST_DROP_SITE_CDASH TRUE)

set(CTEST_PROJECT_SUBPROJECTS
develop
)
