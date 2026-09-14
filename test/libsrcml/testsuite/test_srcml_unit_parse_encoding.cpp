// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_unit_parse_encoding.cpp
 *
 * @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for the source encoding of srcml_unit_parse_*(), both as set with
 * srcml_unit_set_src_encoding() and as detected from the source when it is not.
 * Only UTF-8 is detectable, as every byte sequence is valid ISO-8859-1, so what is
 * not UTF-8 falls back to ISO-8859-1. UTF-16 and UTF-32 are found from a BOM, or
 * from where the NUL bytes of their ASCII characters fall when there is none.
 */

#include <srcml.h>

#include <string>

#include <dassert.hpp>

int main(int, char* argv[]) {

    const std::string srcml_start = R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++"><comment type="line">)";
    const std::string srcml_end = R"(</comment>
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";
    const std::string srcml_comment = srcml_start + u8"// café naïve" + srcml_end;
    const std::string srcml_wide = srcml_start + u8"// é" + srcml_end;
    const std::string srcml_control = srcml_start + "// \xc2\x93quoted\xc2\x94 \xc2\x97 dash" + srcml_end;
    const std::string srcml_empty = R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++"/>)";
    const std::string srcml_swapped = R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++"><expr><name>)" u8"\u2F00\u2F00\u2000\uE900\u0A00\u6100\u3B00\u0A00" R"(</name></expr></unit>)";

    const std::string control = "// \x93quoted\x94 \x97 dash\na;\n";
    const std::string invalid = "// caf\xe9\na;\n";
    const std::string bom = "\xef\xbb\xbf// caf\xc3\xa9 na\xc3\xafve\na;\n";
    const std::string bom16le("\xff\xfe\x2f\x00\x2f\x00\x20\x00\xe9\x00\x0a\x00\x61\x00\x3b\x00\x0a\x00", 18);
    const std::string bom16be("\xfe\xff\x00\x2f\x00\x2f\x00\x20\x00\xe9\x00\x0a\x00\x61\x00\x3b\x00\x0a", 18);
    const std::string bom32le("\xff\xfe\x00\x00\x2f\x00\x00\x00\x2f\x00\x00\x00\x20\x00\x00\x00\xe9\x00\x00\x00\x0a\x00\x00\x00\x61\x00\x00\x00\x3b\x00\x00\x00\x0a\x00\x00\x00", 36);
    const std::string bom32be("\x00\x00\xfe\xff\x00\x00\x00\x2f\x00\x00\x00\x2f\x00\x00\x00\x20\x00\x00\x00\xe9\x00\x00\x00\x0a\x00\x00\x00\x61\x00\x00\x00\x3b\x00\x00\x00\x0a", 36);

    /*
      detected
    */

    // valid UTF-8, so detected as UTF-8 and passed through unchanged
    {
        const std::string src = "// caf\xc3\xa9 na\xc3\xafve\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_comment);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-8");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // the same characters in ISO-8859-1, which is not valid UTF-8, so they are converted
    {
        const std::string src = "// caf\xe9 na\xefve\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_comment);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // data that is not valid UTF-8 for reasons the lead byte alone does not show,
    // each of which is the fallback to ISO-8859-1, where every one of these bytes is
    // a character of its own
    // the cases are separate, as the check stops at the first one it finds

    // an overlong encoding, here of '/', which UTF-8 requires to be a single byte
    {
        const std::string src = "// \xc0\xaf\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + "// \xc3\x80\xc2\xaf" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // a UTF-16 surrogate, which is not a character that UTF-8 encodes
    {
        const std::string src = "// \xed\xa0\x80\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + "// \xc3\xad\xc2\xa0\xc2\x80" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // past U+10FFFF, which is the largest character there is
    {
        const std::string src = "// \xf5\x80\x80\x80\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + "// \xc3\xb5\xc2\x80\xc2\x80\xc2\x80" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // a continuation byte with no lead byte before it
    {
        const std::string src = "// \x80\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + "// \xc2\x80" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // a lead byte with its continuation bytes cut off by the end of the source, which is
    // not an error while there is more data to read, and is one once there is not
    {
        const std::string src = "// caf\xc3\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + "// caf\xc3\x83" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // not UTF-8, so ISO-8859-1, including the 0x80 - 0x9F range, which holds the C1
    // control characters there and so passes these bytes through as those characters
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, control.data(), control.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_control);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      specified
    */

    // a specified encoding is used as is, with no detection, which for this data
    // is what detecting it arrives at anyway
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_parse_memory(unit, control.data(), control.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_control);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // a specified encoding is not a guess, so data that is not valid in it is an error,
    // rather than the fallback that a detected encoding gets
    // the conversion from UTF-8 to UTF-8 is a pass through that does not itself reject
    // anything, so without this check these bytes would go straight into the srcML
    // and leave it not valid XML
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_unit_parse_memory(unit, invalid.data(), invalid.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_empty);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // the same for an encoding whose conversion to UTF-8 is not a pass through, so it is
    // the conversion itself that rejects the data
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "ASCII");
        srcml_unit_parse_memory(unit, invalid.data(), invalid.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_empty);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // the same data with no encoding specified is detected as ISO-8859-1 instead
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, invalid.data(), invalid.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + "// caf\xc3\xa9" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      UTF-8 BOM
    */

    // a UTF-8 BOM is used as is, with no detection
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, bom.data(), bom.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_comment);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-8");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // an encoding is identified by a single name, so a specified encoding spelled as an
    // alias of the one the BOM indicates is that same encoding, and not a mismatch
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF8");
        srcml_unit_parse_memory(unit, bom.data(), bom.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_comment);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-8");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "utf-8");
        srcml_unit_parse_memory(unit, bom.data(), bom.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_comment);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-8");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // any other specified encoding is a mismatch, and is still followed,
    // with the BOM skipped, as it is not part of the source
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
        srcml_unit_parse_memory(unit, bom.data(), bom.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + u8"// cafÃ© naÃ¯ve" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      UTF-16 and UTF-32 with no BOM
    */

    // where the NUL bytes of the ASCII characters give both the width of a character
    // and the byte order
    // the bytes are given directly, as the point of these is where the NUL bytes fall
    {
        const std::string src("\x2f\x00\x2f\x00\x20\x00\xe9\x00\x0a\x00\x61\x00\x3b\x00\x0a\x00", 16);

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string src("\x00\x2f\x00\x2f\x00\x20\x00\xe9\x00\x0a\x00\x61\x00\x3b\x00\x0a", 16);

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16BE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string src("\x2f\x00\x00\x00\x2f\x00\x00\x00\x20\x00\x00\x00\xe9\x00\x00\x00\x0a\x00\x00\x00\x61\x00\x00\x00\x3b\x00\x00\x00\x0a\x00\x00\x00", 32);

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-32LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        const std::string src("\x00\x00\x00\x2f\x00\x00\x00\x2f\x00\x00\x00\x20\x00\x00\x00\xe9\x00\x00\x00\x0a\x00\x00\x00\x61\x00\x00\x00\x3b\x00\x00\x00\x0a", 32);

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-32BE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // a stray NUL byte in data that is otherwise ASCII is not enough of them to be
    // a wide encoding, which would take one for every character
    {
        const std::string src("// a\x00" "b\na;\n", 10);

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + R"(// a<escape char="0x00"/>b)" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-8");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // a character outside the BMP is a surrogate pair, whose low-order byte is NUL and
    // so falls where the data bytes of the other characters are, which is why nearly all
    // of a position being NUL, rather than all of it, is what makes a wide encoding
    {
        const std::string src("\x2f\x00\x2f\x00\x20\x00\x3d\xd8\x00\xde\x20\x00\x78\x00\x0a\x00\x61\x00\x3b\x00\x0a\x00", 22);

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + u8"// 😀 x" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      UTF-16 and UTF-32 BOM
    */

    // a BOM gives the byte order directly, and is not part of the source
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, bom16le.data(), bom16le.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, bom16be.data(), bom16be.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16BE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // the UTF-32LE BOM starts with the UTF-16LE one, so it has to be found first
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, bom32le.data(), bom32le.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-32LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, bom32be.data(), bom32be.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-32BE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // the encoding of the BOM may be specified with or without its byte order, as the
    // BOM indicates both, and is not a mismatch either way
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF-16");
        srcml_unit_parse_memory(unit, bom16le.data(), bom16le.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF-16LE");
        srcml_unit_parse_memory(unit, bom16le.data(), bom16le.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF-16BE");
        srcml_unit_parse_memory(unit, bom16be.data(), bom16be.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16BE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF-32");
        srcml_unit_parse_memory(unit, bom32le.data(), bom32le.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-32LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF-32LE");
        srcml_unit_parse_memory(unit, bom32le.data(), bom32le.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-32LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF-32BE");
        srcml_unit_parse_memory(unit, bom32be.data(), bom32be.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_wide);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-32BE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // while the other byte order is a mismatch, and is still followed,
    // so that each pair of bytes is read as the wrong character
    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_src_encoding(unit, "UTF-16BE");
        srcml_unit_parse_memory(unit, bom16le.data(), bom16le.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_swapped);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16BE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      more than one read of the source
    */

    // a multibyte character split across the end of the first read of the source, so that
    // the data read after it starts partway through that character
    // "// " and 1020 more characters put the first byte of the é at 1023, and the second
    // at 1024, which is where the next read of the source starts
    {
        const std::string padding(1020, 'x');
        const std::string src = "// " + padding + "\xc3\xa9\xc3\xa9 tail\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + "// " + padding + "\xc3\xa9\xc3\xa9 tail" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-8");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // the same for a conversion that is not a pass through, where a surrogate pair takes
    // four bytes and so can be split even though every other character is two bytes
    // the source is UTF-16LE, so each ASCII character is followed by a NUL byte
    {
        const std::string padding(508, 'x');
        std::string src;
        for (char c : "// " + padding) {
            src += c;
            src += '\0';
        }
        src += std::string("\x3d\xd8\x00\xde", 4);
        for (char c : std::string(" tail\na;\n")) {
            src += c;
            src += '\0';
        }

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_start + "// " + padding + u8"😀 tail" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "UTF-16LE");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // the encoding is detected from the first read of the source, which is longer than
    // this, so that the data only stops being valid UTF-8 well after where it is detected
    const std::string padding(2000, 'x');
    const std::string srcml_late_start = srcml_start + "// " + padding + R"(</comment>
<comment type="line">)";

    // corrected to ISO-8859-1 as the rest of the source is read
    {
        const std::string src = "// " + padding + "\n// caf\xe9 na\xefve\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_late_start + u8"// café naïve" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    // corrected to ISO-8859-1 over the 0x80 - 0x9F range as the rest of the source is read
    {
        const std::string src = "// " + padding + "\n// \x93quoted\x94 \x97 dash\na;\n";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_filename(archive, "project.xml");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, src.data(), src.size());
        dassert(srcml_unit_get_srcml_outer(unit), srcml_late_start + "// \xc2\x93quoted\xc2\x94 \xc2\x97 dash" + srcml_end);
        dassert(std::string(srcml_unit_get_src_encoding(unit)), "ISO-8859-1");

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    srcml_cleanup_globals();

    return 0;
}
