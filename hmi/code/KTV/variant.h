#ifndef KTV_VARIANT_H
#define KTV_VARIANT_H

#include <string>

namespace KTV {

/**
 * @brief The value of a KTV attribute with type information
 *
 * KTV attribute values consist of the raw value and an encoding, both strings.
 * The encoding indicates what the attribute value's "real" type is, and how to translate between
 * the string representation and the real type.
 *
 * KTV does not standardize any encodings, but a few are common in practice:
 * @a b for bool, @a n for numbers (floating point or integer), @a b64 for binary data
 * @a s for string.
 *
 * This class handles the listed encodings.
 */

class Variant {
public:
    /// Constructs an invalid attribute (isValid() returns false).
    Variant();

    Variant(const Variant &other);

    /// Constructs a Variant containing an arbitrary type that you must encode to QByteArray
    /// @p value before passing it in, together with the @p encoding used.
    /// @attention @p encoding and @p value may @em not contain null or '@n' byte values; additionally,
    /// @p encoding may not contain ':'.
    Variant(const std::string &value, const std::string &encoding);

    /// Constructs a Variant containing a string (empty encoding).
    Variant(const std::string &value);

    /// Constructs a Variant containing a bool ("b" encoding).
    Variant(bool value);

    /// Constructs a Variant containing an int ("n" encoding).
    Variant(int value);

    /// Constructs a Variant containing a double ("n" encoding).
    Variant(double value);

    virtual ~Variant();

    /// Returns true if the variant is valid, false otherwise.
    /// So far only the default constructor can create invalid variant.
    bool isValid() const;

    /// Returns the raw value.
    std::string value() const;

    /// Returns the encoding, indicating how to transform the raw string value to a different type.
    /// If the encoding is empty, the raw value is probably meant to be used as is.
    std::string encoding() const;

    /// Like value(), but checks that the encoding is either empty or "s".
    std::string toString() const;

    /// Converts to bool (encoding "b").
    bool toBool() const;

    /**
     * Converts to int (encoding "n").
     *
     * @param ok If not null: If a conversion error occurs, *ok is set to false, otherwise *ok is set to true
     */
    int toInt() const;

    /**
     * Converts to double (encoding "n").
     * @param ok If not null: If a conversion error occurs, *ok is set to false, otherwise *ok is set to true
     */
    double toDouble() const;

    /**
     * Converts to a JSON document (encoding "json").
     * @param error If not null: If a parsing error occurs, *error contains the details
     */
    //    char* toJson() const;
    /// Tests whether @p other is equal to this variant (validity, encoding and value)
    bool operator==(const Variant &other) const;

    /// Tests whether @p other is unequal to this variant (validity, encoding and value)
    bool operator!=(const Variant &other) const;

    void operator=(const Variant &other);

private:
    class Private;
    Private* d;
};

}

#endif // KTV_VARIANT_H
