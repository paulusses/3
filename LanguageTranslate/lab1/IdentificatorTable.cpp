#include"IdentificatorTable.h"

std::string Identificator::get_name() {
    return name;
}

void Identificator::set_name(std::string Fname) {
    name = Fname;
}

int Identificator::get_type() {
    return TYPE;
}

void Identificator::set_type(int ID_Type) {
    TYPE = ID_Type;
} 

bool Identificator::get_value() {
    return value;
}

void Identificator::set_value(bool assign) {
    value = assign;
}

IdentificatorTable::IdentificatorTable() {
}

IdentificatorTable::IdentificatorTable(int size) {
    table.resize(size);
}

int IdentificatorTable::CheckInTable(const std::string name) {
    for (int i = 0; i < table.size(); i++) {
        if (table[i].get_name() == name ) {
                return i;
            }
        }
    return -1;
}

bool IdentificatorTable::insert(const std::string& name, int t, bool val) {
    if (CheckInTable(name) == -1 ) {
        Identificator point;
        point.set_name(name);
        point.set_type(t);
        point.set_value(val);
        table.push_back(point);
        return true;
    } else {
        return false;
    }
}

void IdentificatorTable::lookup(const std::string name) {
    int ptr = CheckInTable(name);
    if (ptr) {
        PrintOnePoint(table[ptr]);
    } else {
        std::cout << "not found" << std::endl;
    }
}

int IdentificatorTable::ChangeTypeWithIndex(int ind, int t) {
    if (ind > table.size()) {
        return -1;
    } else {
        if (t > 0 && t < 4) {
            table[ind].set_type(t);
            return 1;
        } else {
            return -2;
        }
    }
}


int IdentificatorTable::ChangeTypeWithName(std::string name, int t) {
    int ptr = CheckInTable(name);
    if (ptr != -1) {
        if (t > 0 && t < 4) {
            table[ptr].set_type(t);
            return 1;
        }else {
            return -2;
        }
    } else {
        return -1;
    }
}

void IdentificatorTable::PrintOnePoint(Identificator p) {
    std::cout << " NAME :  " << p.get_name();
    std::cout << "| TYPE : " << p.get_type();
    std::cout << "| VALUE : " << p.get_value() << std::endl;
}

int IdentificatorTable::ChangeNameWithIndex(int ind, std::string name) {
    if (ind < table.size()) {
        table[ind].set_name(name);
        return 1;
    } else {
        return -1;
    }
}

int IdentificatorTable::ChangeNameWithName(std::string nameold, std::string namenew) {
    int ptr = CheckInTable(nameold);
    if (ptr != -1) {
        table[ptr].set_name(namenew);
        return 1;
    } else {
        return ptr;
    }
}

int IdentificatorTable::ChangeValueWithIndex(int ind, bool ass) {
    if (ind < table.size()) {
        table[ind].set_value(ass);
        return 1;
    } else {
        return -1;
    }
}

int IdentificatorTable::ChangeValueWithName(std::string name, bool ass) {
    int ptr = CheckInTable(name);
    if (ptr != -1) {
        table[ptr].set_value(ass);
        return 1;
    } else {
        return ptr;
    } 
}

void IdentificatorTable::PrintAllTable(std::string file) {
    std::ofstream f(file);
    for (int i = 0; i < table.size(); i++) {
        f << "ID: " << i;
        f << " | NAME: " << table[i].get_name();
        f << " | TYPE: " << table[i].get_type();
        f << " | VALUE: " << table[i].get_value() << std::endl;
    }
    f.close();
}
