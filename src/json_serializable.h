#ifndef JSON_SERIALIZABLE_H_
#define JSON_SERIALIZABLE_H_

#include "json.hpp"

class json_serializable {
public:
    virtual ~json_serializable() { };

    virtual nlohmann::json as_json() const = 0;
};

#endif /* JSON_SERIALIZABLE_H_ */
