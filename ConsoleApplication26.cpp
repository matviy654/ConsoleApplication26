#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct Violation {
    std::string date;
    std::string description;

    Violation(const std::string& date, const std::string& description)
        : date(date), description(description) {}
};

class Node {
public:
    std::string carNumber;  // Ключ - номер машини
    std::vector<Violation> violations;  // Список правопорушень
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(const std::string& carNumber, const Violation& violation)
        : carNumber(carNumber) {
        violations.push_back(violation);
    }
};

class TrafficDatabase {
private:
    std::unique_ptr<Node> root;

    // Вставка нового порушення в дерево
    void addViolation(Node*& node, const std::string& carNumber, const Violation& violation) {
        if (!node) {
            node = new Node(carNumber, violation);
        }
        else if (carNumber < node->carNumber) {
            addViolation(node->left.get(), carNumber, violation);
        }
        else if (carNumber > node->carNumber) {
            addViolation(node->right.get(), carNumber, violation);
        }
        else {
            node->violations.push_back(violation);  // Додаємо порушення до існуючого вузла
        }
    }

    // Рекурсивний друк всього дерева
    void printAll(const Node* node) const {
        if (!node) return;
        printAll(node->left.get());
        std::cout << "Car Number: " << node->carNumber << std::endl;
        for (const auto& violation : node->violations) {
            std::cout << "  Date: " << violation.date << ", Description: " << violation.description << std::endl;
        }
        printAll(node->right.get());
    }

    // Пошук даних за заданим номером
    const Node* findByCarNumber(const Node* node, const std::string& carNumber) const {
        if (!node) return nullptr;
        if (carNumber < node->carNumber) return findByCarNumber(node->left.get(), carNumber);
        if (carNumber > node->carNumber) return findByCarNumber(node->right.get(), carNumber);
        return node;
    }

    // Друк даних за діапазоном номерів
    void printByRange(const Node* node, const std::string& start, const std::string& end) const {
        if (!node) return;
        if (start < node->carNumber) printByRange(node->left.get(), start, end);
        if (start <= node->carNumber && node->carNumber <= end) {
            std::cout << "Car Number: " << node->carNumber << std::endl;
            for (const auto& violation : node->violations) {
                std::cout << "  Date: " << violation.date << ", Description: " << violation.description << std::endl;
            }
        }
        if (end > node->carNumber) printByRange(node->right.get(), start, end);
    }

public:
    TrafficDatabase() : root(nullptr) {}

    void addViolation(const std::string& carNumber, const Violation& violation) {
        addViolation(root.get(), carNumber, violation);
    }

    void printAll() const {
        if (!root) {
            std::cout << "Database is empty." << std::endl;
        }
        else {
            printAll(root.get());
        }
    }

    void printByCarNumber(const std::string& carNumber) const {
        const Node* node = findByCarNumber(root.get(), carNumber);
        if (!node) {
            std::cout << "No records found for car number: " << carNumber << std::endl;
        }
        else {
            std::cout << "Car Number: " << node->carNumber << std::endl;
            for (const auto& violation : node->violations) {
                std::cout << "  Date: " << violation.date << ", Description: " << violation.description << std::endl;
            }
        }
    }

    void printByRange(const std::string& start, const std::string& end) const {
        if (!root) {
            std::cout << "Database is empty." << std::endl;
        }
        else {
            printByRange(root.get(), start, end);
        }
    }
};

int main() {
    TrafficDatabase db;

    db.addViolation("AB1234", Violation("2023-11-11", "Speeding"));
    db.addViolation("AB1234", Violation("2023-11-12", "Parking in a no-parking zone"));
    db.addViolation("CD5678", Violation("2023-11-13", "Driving without a seatbelt"));
    db.addViolation("EF9101", Violation("2023-11-14", "Running a red light"));

    std::cout << "Full Database Print:" << std::endl;
    db.printAll();

    std::cout << "\nPrint by Car Number (AB1234):" << std::endl;
    db.printByCarNumber("AB1234");

    std::cout << "\nPrint by Range (AB0000 to CD9999):" << std::endl;
    db.printByRange("AB0000", "CD9999");

    return 0;
}
