#include <iostream>
#include <string>
#include <algorithm>

struct Payment {
    double amount;
    std::string description;
};

struct Transfer {
    double amount;
    std::string from;
    std::string to;
};

struct Exchange {
    double amount;
    std::string fromCurrency;
    std::string toCurrency;
    double rate;
};

enum class TransactionType {
    PAYMENT,
    TRANSFER,
    EXCHANGE
};


class Transaction {
private:
    void* transaction_data;
    TransactionType type;
    bool is_completed;

    void freeData() {
        if (transaction_data == nullptr)
            return;

        switch (type) {
        case TransactionType::PAYMENT:
            delete static_cast<Payment*>(transaction_data);
            break;

        case TransactionType::TRANSFER:
            delete static_cast<Transfer*>(transaction_data);
            break;

        case TransactionType::EXCHANGE:
            delete static_cast<Exchange*>(transaction_data);
            break;
        }

        transaction_data = nullptr;
    }

    void copyData(const Transaction& other) {
        
        if (other.transaction_data == nullptr) {
            transaction_data = nullptr;
            return;
        }

        switch (other.type) {
        case TransactionType::PAYMENT:
            transaction_data =
                new Payment(*static_cast<Payment*>(other.transaction_data));
            break;

        case TransactionType::TRANSFER:
            transaction_data =
                new Transfer(*static_cast<Transfer*>(other.transaction_data));
            break;

        case TransactionType::EXCHANGE:
            transaction_data =
                new Exchange(*static_cast<Exchange*>(other.transaction_data));
            break;
        }
    }

public:
    Transaction(): transaction_data(nullptr), type(TransactionType::PAYMENT), is_completed(false){
        std::cout << "Transaction: default constructor\n";
    }

    Transaction(const Payment& payment): transaction_data(new Payment(payment)), 
          type(TransactionType::PAYMENT), is_completed(false) {
        std::cout << "Transaction: Payment constructor\n";
    }

    Transaction(const Transfer& transfer): transaction_data(new Transfer(transfer)),
          type(TransactionType::TRANSFER), is_completed(false){
        std::cout << "Transaction: Transfer constructor\n";
    }

    Transaction(const Exchange& exchange) : transaction_data(new Exchange(exchange)),
          type(TransactionType::EXCHANGE), is_completed(false) {
        std::cout << "Transaction: Exchange constructor\n";
    }

    Transaction(const Transaction& other): transaction_data(nullptr),
          type(other.type), is_completed(other.is_completed){
        copyData(other);

        std::cout << "Transaction: copy constructor\n";
    }

    Transaction& operator=(const Transaction& other) {
        std::cout << "Transaction: copy assignment\n";

        if (this == &other)
            return *this;

        freeData();
        type = other.type;
        is_completed = other.is_completed;

        copyData(other);

        return *this;
    }

    Transaction(Transaction&& other) noexcept
        : transaction_data(other.transaction_data),
          type(other.type), is_completed(other.is_completed){
        other.transaction_data = nullptr;
        other.is_completed = false;

        std::cout << "Transaction: move constructor\n";
    }
    
    Transaction& operator=(Transaction&& other) noexcept {
        std::cout << "Transaction: move assignment\n";

        if (this == &other)
            return *this;

        freeData();

        transaction_data = other.transaction_data;
        type = other.type;
        is_completed = other.is_completed;

        other.transaction_data = nullptr;
        other.is_completed = false;

        return *this;
    }

    ~Transaction() {
        freeData();

        std::cout << "Transaction: destructor\n";
    }

    void process() {is_completed = true;}
    TransactionType getType() const {return type;}
    bool isCompleted() const {return is_completed;}

    void print() const {
        std::cout << "Type: ";

        switch (type) {

        case TransactionType::PAYMENT: {
            std::cout << "PAYMENT\n";

            Payment* payment =
                static_cast<Payment*>(transaction_data);

            std::cout << "  Amount: " << payment->amount << "\n";
            std::cout << "  Description: "
                      << payment->description << "\n";

            break;
        }

        case TransactionType::TRANSFER: {
            std::cout << "TRANSFER\n";

            Transfer* transfer =
                static_cast<Transfer*>(transaction_data);

            std::cout << "  Amount: " << transfer->amount << "\n";
            std::cout << "  From: " << transfer->from << "\n";
            std::cout << "  To: " << transfer->to << "\n";

            break;
        }

        case TransactionType::EXCHANGE: {
            std::cout << "EXCHANGE\n";

            Exchange* exchange =
                static_cast<Exchange*>(transaction_data);

            std::cout << "  Amount: " << exchange->amount << "\n";
            std::cout << "  From currency: "
                      << exchange->fromCurrency << "\n";
            std::cout << "  To currency: "
                      << exchange->toCurrency << "\n";
            std::cout << "  Rate: "
                      << exchange->rate << "\n";

            break;
        }
        }

        std::cout << "  Completed: "
                  << (is_completed ? "yes" : "no")
                  << "\n";
    }
};

class TransactionProcessor {
private:
    Transaction* transactions;
    size_t count;
    size_t capacity;


    void resize(size_t newCapacity) {
        Transaction* newTransactions =
            new Transaction[newCapacity];

        for (size_t i = 0; i < count; ++i) {
            newTransactions[i] = std::move(transactions[i]);
        }

        delete[] transactions;

        transactions = newTransactions;
        capacity = newCapacity;
    }

public:
    TransactionProcessor(): transactions(nullptr), count(0), capacity(0)
    {
        std::cout << "Processor: default constructor\n";
    }

    TransactionProcessor(size_t initialCapacity): transactions(nullptr), count(0), capacity(initialCapacity){
        if (capacity > 0)
            transactions = new Transaction[capacity];

        std::cout << "Processor: parameterized constructor\n";
    }

    TransactionProcessor(const TransactionProcessor& other): transactions(nullptr),
          count(other.count), capacity(other.capacity){
        if (capacity > 0) {
            transactions = new Transaction[capacity];

            for (size_t i = 0; i < count; ++i) {
                transactions[i] = other.transactions[i];
            }
        }

        std::cout << "Processor: copy constructor\n";
    }

    TransactionProcessor& operator=(
        const TransactionProcessor& other)
    {
        std::cout << "Processor: copy assignment\n";

        if (this == &other)
            return *this;

        Transaction* newTransactions = nullptr;

        if (other.capacity > 0) {
            newTransactions =
                new Transaction[other.capacity];

            for (size_t i = 0; i < other.count; ++i) {
                newTransactions[i] = other.transactions[i];
            }
        }

        delete[] transactions;

        transactions = newTransactions;
        count = other.count;
        capacity = other.capacity;

        return *this;
    }

    
    TransactionProcessor(TransactionProcessor&& other) noexcept
        : transactions(other.transactions), count(other.count), capacity(other.capacity){
        other.transactions = nullptr;
        other.count = 0;
        other.capacity = 0;

        std::cout << "Processor: move constructor\n";
    }

    
    TransactionProcessor& operator=(
        TransactionProcessor&& other) noexcept
    {
        std::cout << "Processor: move assignment\n";

        if (this == &other)
            return *this;

        delete[] transactions;

        transactions = other.transactions;
        count = other.count;
        capacity = other.capacity;

        other.transactions = nullptr;
        other.count = 0;
        other.capacity = 0;

        return *this;
    }

    
    ~TransactionProcessor() {
        delete[] transactions;

        std::cout << "Processor: destructor\n";
    }

    
    void addTransaction(const Transaction& transaction) {

        if (count >= capacity) {
            size_t newCapacity =
                (capacity == 0) ? 2 : capacity * 2;

            resize(newCapacity);
        }

        transactions[count] = transaction;
        ++count;
    }

    
    void processTransaction(size_t index) {

        if (index >= count) {
            std::cout << "Invalid transaction index\n";
            return;
        }

        transactions[index].process();
    }

    
    void printReport() const {

        std::cout << "\n========== TRANSACTION REPORT ==========\n";

        if (count == 0) {
            std::cout << "No transactions.\n";
            return;
        }

        for (size_t i = 0; i < count; ++i) {

            std::cout << "\nTransaction #" << i + 1 << "\n";

            transactions[i].print();
        }

        std::cout << "\n========================================\n";
    }

    
    void findByType(TransactionType searchType) const {

        std::cout << "\nTransactions of requested type:\n";

        bool found = false;

        for (size_t i = 0; i < count; ++i) {

            if (transactions[i].getType() == searchType) {

                std::cout << "\nTransaction #" << i + 1 << "\n";

                transactions[i].print();

                found = true;
            }
        }

        if (!found) {
            std::cout << "No transactions found.\n";
        }
    }

    
    void printStatistics() const {

        size_t completedPayments = 0;
        size_t completedTransfers = 0;
        size_t completedExchanges = 0;

        for (size_t i = 0; i < count; ++i) {

            if (!transactions[i].isCompleted())
                continue;

            switch (transactions[i].getType()) {

            case TransactionType::PAYMENT:
                ++completedPayments;
                break;

            case TransactionType::TRANSFER:
                ++completedTransfers;
                break;

            case TransactionType::EXCHANGE:
                ++completedExchanges;
                break;
            }
        }

        std::cout << "\n========== STATISTICS ==========\n";

        std::cout << "Completed payments: "
                  << completedPayments << "\n";

        std::cout << "Completed transfers: "
                  << completedTransfers << "\n";

        std::cout << "Completed exchanges: "
                  << completedExchanges << "\n";

        std::cout << "================================\n";
    }
};


int main() {

    TransactionProcessor processor(2);

    Payment payment{1500.0, "Payment for university courses"};
    Transaction t1(payment);

    Transfer transfer{5000.0, "Alice", "Bob"};
    Transaction t2(transfer);

    Exchange exchange{100.0, "USD", "EUR", 0.92};
    Transaction t3(exchange);

    processor.addTransaction(t1);
    processor.addTransaction(t2);
    processor.addTransaction(t3);
    
    processor.processTransaction(0);
    processor.processTransaction(2);


    processor.printReport();
    processor.findByType(TransactionType::PAYMENT);
    processor.printStatistics();

    std::cout << "\n========== COPY TEST ==========\n";
    TransactionProcessor copyProcessor = processor;
    copyProcessor.printReport();


    std::cout << "\n========== MOVE TEST ==========\n";
    TransactionProcessor movedProcessor =
        std::move(processor);
    movedProcessor.printReport();


    return 0;
}