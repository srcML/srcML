#include <srcmlns.hpp>

// namespace form of immediate add
// * Update prefixes
// * Add an new uri's
// * Or flags
Namespaces& operator +=(Namespaces& namespaces, const Namespaces& otherns) {

    for (const auto& ns : otherns) {

        // find where the new URI is in the default URI list, or not
        auto&& view = namespaces.get<nstags::uri>();
        auto it = view.find(ns.uri);
        if (it != view.end()) {

            // update the default prefix
            view.modify(it, [ns](Namespace& thisns){ thisns.prefix = ns.prefix; thisns.flags |= ns.flags; });

        } else {

            // create a new entry for this URI
            namespaces.push_back({ .prefix = ns.prefix, .uri = ns.uri, .flags = ns.flags });
        }
    }

    return namespaces;
}

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
