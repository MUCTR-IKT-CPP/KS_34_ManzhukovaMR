#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>

const int MIN_AGE = 13;
const int MAX_AGE = 80;
const int MIN_FRIENDS = 0;
const int MAX_FRIENDS = 5000;
const int MIN_YEAR = 2004;
const int MAX_YEAR = 2024;
const int MIN_LAST_LOGIN = 0;
const int MAX_LAST_LOGIN = 1000;
const int ALPHABET_SIZE = 26;
const char FIRST_UPPER = 'A';
const char FIRST_LOWER = 'a';
const int MIN_LEN = 5;
const int MAX_LEN = 10;

struct SocialMediaProfile
{
    std::string username;
    int age;
    int number_of_friends;
    int registration_year;
    bool is_premium;
    int last_login;
};

/**
 * Generates a random username made of Latin letters with an index suffix.
 * The first letter is uppercase, the remaining ones are lowercase.
 *
 * @param index The numeric suffix that guarantees the username is unique.
 * @return A newly generated username string.
 */
std::string generateUsername(int index)
{
    int len = MIN_LEN + rand() % (MAX_LEN - MIN_LEN + 1);
    std::string name;
    name += FIRST_UPPER + rand() % ALPHABET_SIZE;
        for (int i = 1; i < len; i++)
            name += FIRST_LOWER + rand() % ALPHABET_SIZE;

    name += std::to_string(index);
    return name;
}

/**
 * Fills one profile with randomly generated data within the allowed ranges.
 *
 * @param p     Reference to the profile that will be filled.
 * @param index Index of the profile, used to build a unique username.
 */
void generateOne(SocialMediaProfile &p, int index)
{
    p.username = generateUsername(index);
    p.age = MIN_AGE + rand() % (MAX_AGE - MIN_AGE + 1);
    p.number_of_friends = MIN_FRIENDS + rand() % (MAX_FRIENDS - MIN_FRIENDS + 1);
    p.registration_year = MIN_YEAR + rand() % (MAX_YEAR - MIN_YEAR + 1);
    p.is_premium = (rand() % 2 == 1);
    p.last_login = MIN_LAST_LOGIN + rand() % (MAX_LAST_LOGIN - MIN_LAST_LOGIN + 1);
}

/**
 * Fills an array of profiles with random data.
 *
 * @param arr Pointer to the array of profiles.
 * @param n   Number of elements in the array.
 */
void generateSocialMediaProfile(SocialMediaProfile *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        generateOne(arr[i], i);
    }
}

/**
 * Prints all profiles of the array to standard output, one field per line.
 *
 * @param arr Pointer to the array of profiles.
 * @param n   Number of elements to print.
 */
void printSocialMediaProfile(SocialMediaProfile *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << i + 1 << ") "
                  << arr[i].username << "\n"
                  << "age = " << arr[i].age << "\n"
                  << "friends = " << arr[i].number_of_friends << "\n"
                  << "year = " << arr[i].registration_year << "\n"
                  << "premium = " << (arr[i].is_premium ? "yes" : "no") << "\n"
                  << "last_login = " << arr[i].last_login << " d\n";
    }
}

/**
 * Prints the average age, average number of friends,
 * and total number of premium users in the array.
 *
 * @param arr Pointer to the array of profiles.
 * @param n   Number of elements in the array.
 */
void analyzeSocialMediaProfile(SocialMediaProfile *arr, int n)
{
    double sum_age = 0, sum_friends = 0;
    int premium_count = 0;
    for (int i = 0; i < n; i++)
    {
        sum_age += arr[i].age;
        sum_friends += arr[i].number_of_friends;
        if (arr[i].is_premium)
            premium_count++;
    }
    std::cout << "Average age: " << sum_age / n << "\n";
    std::cout << "Average friends: " << sum_friends / n << "\n";
    std::cout << "Premium users: " << premium_count << "\n";
}

/**
 * Prints every user whose last login was more than x days ago.
 * If no such user exists, prints a message about it.
 *
 * @param arr Pointer to the array of profiles.
 * @param n   Number of elements in the array.
 * @param x   Threshold of inactivity in days.
 */
void findInactiveUsers(SocialMediaProfile *arr, int n, int x)
{
    bool found = false;
    for (int i = 0; i < n; i++)
    {
        if (arr[i].last_login > x)
        {
            std::cout << arr[i].username
                      << " (last login: " << arr[i].last_login << " days)\n";
            found = true;
        }
    }
    if (!found)
        std::cout << "No inactive users.\n";
}

/**
 * Copies users registered not later than the given year into the result array
 * and sorts them by the number of friends in descending order.
 *
 * @param arr    Pointer to the source array of profiles.
 * @param n      Number of elements in the source array.
 * @param year   Maximum registration year to include in the result.
 * @param result Pointer to the array where filtered profiles are stored.
 * @return The number of profiles actually placed into the result array.
 */
int oldsSocialMediaProfile(SocialMediaProfile *arr, int n, int year,
                           SocialMediaProfile *result)
{
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        if (arr[i].registration_year <= year)
        {
            result[j] = arr[i];
            j++;
        }
    }
    for (int i = 0; i < j - 1; i++)
        for (int k = 0; k < j - 1 - i; k++)
            if (result[k].number_of_friends < result[k + 1].number_of_friends)
                std::swap(result[k], result[k + 1]);
    return j;
}

/**
 * Returns a lowercase copy of the given string.
 *
 * @param s Source string.
 * @return A new string with all characters converted to lowercase.
 */
std::string toLower(const std::string &s)
{
    std::string r = s;
    for (char &c : r)
        c = (char)std::tolower((unsigned char)c);
    return r;
}

/**
 * Sorts the array of profiles by username in alphabetical order,
 * ignoring character case (case-insensitive).
 *
 * @param arr Pointer to the array of profiles.
 * @param n   Number of elements in the array.
 */
void sortSocialMediaProfile(SocialMediaProfile *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (toLower(arr[j].username) > toLower(arr[j + 1].username))
                std::swap(arr[j], arr[j + 1]);
}

/**
 * Simulates sending notifications to users inactive for more than 30 days.
 * Prints their usernames and inactivity period to standard output.
 *
 * @param arr Pointer to the array of profiles.
 * @param n   Number of elements in the array.
 */
void sendNotifications(SocialMediaProfile *arr, int n)
{
    std::cout << "--- Sending notifications (last login > 30) ---\n";
    bool found = false;
    for (int i = 0; i < n; i++)
    {
        if (arr[i].last_login > 30)
        {
            std::cout << "-> " << arr[i].username
                      << " (offline " << arr[i].last_login << " days)\n";
            found = true;
        }
    }
    if (!found)
        std::cout << "Everyone is active!\n";
}

/**
 * Program entry point. Reads N, generates profiles,
 * and runs an interactive menu for all available actions.
 *
 * @return 0 on success, 1 if N is not a positive number.
 */
int main()
{
    int num;
    std::cout << "N = ";
    std::cin >> num;
    if (num <= 0)
    {
        std::cout << "N must be positive\n";
        return 1;
    }

    SocialMediaProfile *arr = new SocialMediaProfile[num];
    srand((unsigned)time(nullptr));
    generateSocialMediaProfile(arr, num);
    std::cout << "\n--- Generated data ---\n";
    printSocialMediaProfile(arr, num);

    int choice;
    do
    {
        std::cout << "\n===== MENU =====\n"
                  << "1. Analyze audience\n"
                  << "2. Find inactive users\n"
                  << "3. Find old-timers and sort by friends\n"
                  << "4. Sort by username\n"
                  << "5. Send notifications\n"
                  << "0. Exit\n"
                  << "Your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            analyzeSocialMediaProfile(arr, num);
            break;
        case 2:
        {
            int x;
            std::cout << "X days = ";
            std::cin >> x;
            findInactiveUsers(arr, num, x);
            break;
        }
        case 3:
        {
            int year;
            std::cout << "Year = ";
            std::cin >> year;
            SocialMediaProfile *result = new SocialMediaProfile[num];
            int cnt = oldsSocialMediaProfile(arr, num, year, result);
            if (cnt == 0)
                std::cout << "No old-timers found.\n";
            else
                printSocialMediaProfile(result, cnt);
            delete[] result;
            break;
        }
        case 4:
            sortSocialMediaProfile(arr, num);
            std::cout << "Sorted:\n";
            printSocialMediaProfile(arr, num);
            break;
        case 5:
            sendNotifications(arr, num);
            break;
        case 0:
            std::cout << "Bye!\n";
            break;
        default:
            std::cout << "Wrong choice\n";
        }
    } while (choice != 0);

    delete[] arr;
    return 0;
}