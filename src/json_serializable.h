#ifndef JSON_SERIALIZABLE_H_
#define JSON_SERIALIZABLE_H_

#include "../lib/json.hpp"

class json_serializable {
public:
    virtual ~json_serializable() { };

    /*
     * Must return a json representation of the implementer class so that
     * it can be written to a .json file.
     */
    virtual nlohmann::json as_json() const = 0;
};

#endif /* JSON_SERIALIZABLE_H_ */
