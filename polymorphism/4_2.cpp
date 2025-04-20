#include <iostream>
#include <vector>
#include <memory>
#include <typeinfo>

class Individual {
public:
    virtual void speak() const = 0;
    virtual int id() const = 0;
    virtual ~Individual() = default;
};

class Alice : public Individual {
public:
    void speak() const override {
        std::cout << "Alice\n";
    }
    int id() const override {
        return 0;
    }
};

class Bob : public Individual {
public:
    void speak() const override {
        std::cout << "Bob\n";
    }
    int id() const override {
        return 1;
    }
};

class Casper : public Individual {
public:
    void speak() const override {
        std::cout << "Casper\n";
    }
    int id() const override {
        return 2;
    }
};

// 1
std::vector<Individual*> createIndividuals() {
    std::vector<Individual*> individuals;
    for (int i = 0; i < 3; ++i) individuals.push_back(new Alice());
    for (int i = 0; i < 2; ++i) individuals.push_back(new Bob());
    for (int i = 0; i < 4; ++i) individuals.push_back(new Casper());
    return individuals;
}

// 2
void letThemSpeak(const std::vector<Individual*>& individuals) {
    for (const auto& p : individuals) {
        p->speak();
    }
}

// 3
void deleteIndividuals(std::vector<Individual*>& individuals) {
    for (auto p : individuals) {
        delete p;
    }
    individuals.clear();
}

// 4.1
int identification(Individual* p) {
    return p->id();
}

// 4.2
int identification_dynamic(Individual* p) {
    if (dynamic_cast<Alice*>(p)) return 0;
    if (dynamic_cast<Bob*>(p)) return 1;
    if (dynamic_cast<Casper*>(p)) return 2;
    return -1;
}

// 4.3
int identification_typeid(Individual* p) {
    if (typeid(*p) == typeid(Alice)) return 0;
    if (typeid(*p) == typeid(Bob)) return 1;
    if (typeid(*p) == typeid(Casper)) return 2;
    return -1;
}

// 5
std::vector<std::unique_ptr<Individual>> createIndividualsSmart() {
    std::vector<std::unique_ptr<Individual>> individuals;
    for (int i = 0; i < 3; ++i) individuals.emplace_back(std::make_unique<Alice>());
    for (int i = 0; i < 2; ++i) individuals.emplace_back(std::make_unique<Bob>());
    for (int i = 0; i < 4; ++i) individuals.emplace_back(std::make_unique<Casper>());
    return individuals;
}

void letThemSpeakSmart(const std::vector<std::unique_ptr<Individual>>& individuals) {
    for (const auto& p : individuals) {
        p->speak();
    }
}

int main() {
    auto individuals = createIndividuals();
    letThemSpeak(individuals);

    for (auto p : individuals) {
        std::cout << "  ID: " << identification(p) << ", dynamic_cast: " << identification_dynamic(p) << ", typeid: " << identification_typeid(p) << "\n";
    }

    deleteIndividuals(individuals);

    auto smartIndividuals = createIndividualsSmart();
    letThemSpeakSmart(smartIndividuals);

    return 0;
}
