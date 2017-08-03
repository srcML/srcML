#include <srcmlns.hpp>

/**
 * is_srcml_namespace
 * @param uri the uri to compare
 * @param srcml_uri the srcML URI to compare to
 *
 * Compare uri to the srcML uri idependent of prefix
 * i.e., www.sdml.info or www.srcML.org.
 *
 * @returns if the two uris are equal.
 */
bool is_srcml_namespace(const std::string& uri, const std::string& srcml_uri) {

	if (uri == srcml_uri)
		return true;

	auto uri_suffix = uri;
	for (auto& prefix : SRCML_URI_PREFIX) {
		if (uri.substr(0, prefix.size()) == prefix) {
			uri_suffix = uri.substr(prefix.size());
			break;
		}
	}

	auto srcml_uri_suffix = srcml_uri;
	for (auto& prefix : SRCML_URI_PREFIX) {
		if (srcml_uri.substr(0, prefix.size()) == prefix) {
			srcml_uri_suffix = srcml_uri.substr(prefix.size());
			break;
		}
	}

	return uri_suffix == srcml_uri_suffix;
}

/**
 * srcml_uri_normalize
 * @param uri the uri to normalize
 *
 * Normalize the uri to www.srcML.org
 * 
 * @returns the normalized uri.
 */
std::string& srcml_uri_normalize(std::string& uri) {

	if (uri.substr(0, SRCML_URI_PREFIX[1].size()) == SRCML_URI_PREFIX[1])
		uri = SRCML_URI_PREFIX[0] + uri.substr(SRCML_URI_PREFIX[1].size());

	return uri;
}
