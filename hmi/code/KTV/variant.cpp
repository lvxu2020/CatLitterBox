#include "variant.h"
#include "variant_p.h"
#include <sstream>
#include <iostream>
#include <stdlib.h>

namespace KTV {

static std::string ftoa(double value) {
    std::ostringstream o;
    if (!(o << value))
        return "";
    return o.str();
}

static std::string itoa(int value) {
    std::ostringstream o;
    if (!(o << value))
        return "";
    return o.str();
}

Variant::Private::Private(Variant* parent) :
    p(parent), m_isValid(false) {
}

Variant::Private::~Private() {
}

Variant::Variant() :
    d(new Private(this)) {
    d->m_isValid = false;
}

Variant::Variant(const Variant &other) :
    d(new Private(this)) {
    d->m_value = other.d->m_value;
    d->m_encoding = other.d->m_encoding;
    d->m_isValid = other.d->m_isValid;
}

void Variant::operator=(const Variant &other) {
    d->m_value = other.d->m_value;
    d->m_encoding = other.d->m_encoding;
    d->m_isValid = other.d->m_isValid;
}

Variant::Variant(const std::string &value, const std::string &encoding) :
    d(new Private(this)) {
    d->m_value = value;
    d->m_encoding = encoding;
    d->m_isValid = true;
}

Variant::Variant(const std::string &value) :
    d(new Private(this)) {
    d->m_value = value;
    d->m_encoding = "s";
    d->m_isValid = true;
}

Variant::Variant(bool value) :
    d(new Private(this)) {
    d->m_value = value ? "true" : "false";
    d->m_encoding = "b";
    d->m_isValid = true;
}

Variant::Variant(int value) :
    d(new Private(this)) {
    d->m_value = itoa(value);
    d->m_encoding = "n";
    d->m_isValid = true;
}

Variant::Variant(double value) :
    d(new Private(this)) {
    d->m_value = ftoa(value);
    d->m_encoding = "n";
    d->m_isValid = true;
}

Variant::~Variant() {
    delete d;
    d = NULL;
}

bool Variant::isValid() const {
    return d->m_isValid;
}

std::string Variant::value() const {
    return d->m_value;
}

std::string Variant::encoding() const {
    return d->m_encoding;
}

std::string Variant::toString() const {
    if (!d->m_encoding.empty() && d->m_encoding != "s") {
        //loginfo("Converting to string but encoding is not \"s\": encoding: = [%s] value = [%s]\" \n", d->m_encoding.c_str(), d->m_value.c_str());
    }

    return d->m_value;
}

bool Variant::toBool() const {
    if (d->m_encoding != "b") {
        //loginfo("Converting to string but encoding is not \"b\": encoding: = [%s] value = [%s]\" \n", d->m_encoding.c_str(), d->m_value.c_str());
    }

    return (d->m_value == "true");
}

int Variant::toInt() const {
    if (d->m_encoding != "n") {
        //loginfo("Converting to string but encoding is not \"n\": encoding: = [%s] value = [%s]\" \n", d->m_encoding.c_str(), d->m_value.c_str());
    }

    return atoi(d->m_value.data());
}

double Variant::toDouble() const {
    if (d->m_encoding != "n") {
        //loginfo("Converting to string but encoding is not \"n\": encoding: = [%s] value = [%s]\" \n", d->m_encoding.c_str(), d->m_value.c_str());
    }

    return atof(d->m_value.data());
}

bool Variant::operator==(const Variant &other) const {
    return d->m_isValid == other.d->m_isValid && d->m_value == other.d->m_value
            && d->m_encoding == other.d->m_encoding;
}

bool Variant::operator!=(const Variant &other) const {
    return !operator==(other);
}

}
