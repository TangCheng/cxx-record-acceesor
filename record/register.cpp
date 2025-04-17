#include "register.h"

namespace record {
std::unique_ptr<std::map<std::type_index, FieldMap>> RecordRegister::_instance;
}
