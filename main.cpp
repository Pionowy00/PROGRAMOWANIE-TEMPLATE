
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>
#include <utility>

template<typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template<typename T, typename = void>
struct Wypisywalny : std::false_type {};

template<typename T>
struct Wypisywalny<T, std::void_t<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>> : std::true_type {};

template<typename T, typename = void>
struct maRaport : std::false_type {};

template<typename T>
struct maRaport<T, std::void_t<
    decltype(std::declval<const T&>().raport())
>> : Wypisywalny<decltype(std::declval<const T&>().raport())> {};

template<typename T, typename = void>
struct Kontener : std::false_type {};

template<typename T>
struct Kontener<T, std::void_t<
    decltype(std::declval<const T&>().begin()),
    decltype(std::declval<const T&>().end())
>> : std::integral_constant<bool,
    !std::is_same<remove_cvref_t<T>, std::string>::value> {};

template<typename T>
std::enable_if_t<maRaport<T>::value, std::string>
sformatujDane(const T& obiekt);

template<typename T>
std::enable_if_t<!maRaport<T>::value && Kontener<T>::value, std::string>
sformatujDane(const T& kontener);

template<typename T>
std::enable_if_t<!maRaport<T>::value && !Kontener<T>::value && Wypisywalny<const T&>::value, std::string>
sformatujDane(const T& dane);

template<typename T>
std::enable_if_t<maRaport<T>::value, std::string>
sformatujDane(const T& obiekt) {
    std::ostringstream ss;
    ss << obiekt.raport();
    return ss.str();
}

template<typename T>
std::enable_if_t<!maRaport<T>::value && Kontener<T>::value, std::string>
sformatujDane(const T& kontener) {
    std::ostringstream ss;
    ss << "[";
    bool pierwszy = true;
    for (const auto& element : kontener) {
        if (!pierwszy) ss << ", ";
        ss << sformatujDane(element);
        pierwszy = false;
    }
    ss << "]";
    return ss.str();
}

template<typename T>
std::enable_if_t<!maRaport<T>::value && !Kontener<T>::value && Wypisywalny<const T&>::value, std::string>
sformatujDane(const T& dane) {
    std::ostringstream ss;
    ss << dane;
    return ss.str();
}

class czarnaSkrzynka {
public:
    template<typename... Args>
    void loguj(const std::string& poziom, const std::string& komunikat, const Args&... dane) {
        std::cout << "[" << poziom << "] " << komunikat;
        ((std::cout << " " << sformatujDane(dane)), ...);
        std::cout << '\n';
    }
};

class Student {
public:
    std::string imie;
    int wiek;

    Student(std::string i, int w) : imie(std::move(i)), wiek(w) {}

    std::string raport() const {
        return "Student: " + imie + ", wiek: " + std::to_string(wiek);
    }
};

struct UszkodzonyCzujnik {};

int main() {
    czarnaSkrzynka logger;

    logger.loguj("INFO", "Dane:", 123, 45.67, "tekst");

    std::vector<int> liczby = {1, 2, 3, 4, 5};
    logger.loguj("DEBUG", "Zawartosc wektora:", liczby);

    Student student{"Jan", 21};
    logger.loguj("INFO", "Informacje o studencie:", student);

    logger.loguj("WARNING", "Test mieszany:", 100, liczby, student, "koniec");

    std::string tekst = "Przykladowy napis";
    logger.loguj("INFO", "String:", tekst);

    logger.loguj("ERROR", "Wystapil blad!");

    logger.loguj("INFO", "Zagniezdzony wektor:",
        std::vector<std::vector<int>>{{1, 2}, {3, 4}});

    logger.loguj("INFO", "Wektor studentow:",
        std::vector<Student>{{"Anna", 20}, {"Jan", 21}});

    // logger.loguj("ERROR", "Blad:", UszkodzonyCzujnik{});

    return 0;
}
