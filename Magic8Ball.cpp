#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int main() {
    // Array of classic Magic 8 Ball responses
    const std::string responses[] = {
        // Positive answers
        "It is certain.",
        "It is decidedly so.",
        "Without a doubt.",
        "Yes definitely.",
        "You may rely on it.",
        "As I see it, yes.",
        "Most likely.",
        "Outlook good.",
        "Yes.",
        "Signs point to yes.",
        // Neutral answers
        "Reply hazy, try again.",
        "Ask again later.",
        "Better not tell you now.",
        "Cannot predict now.",
        "Concentrate and ask again.",
        // Negative answers
        "Don't count on it.",
        "My reply is no.",
        "My sources say no.",
        "Outlook not so good.",
        "Very doubtful."
    };

    const int numResponses = sizeof(responses) / sizeof(responses[0]);

    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::string question;

    std::cout << "================================" << std::endl;
    std::cout << "    Welcome to Magic 8 Ball!    " << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Ask a yes/no question and I'll reveal your fate!" << std::endl;
    std::cout << "(Type 'quit' to exit)" << std::endl;
    std::cout << std::endl;

    while (true) {
        std::cout << "Ask your question: ";
        std::getline(std::cin, question);

        // Check if user wants to quit
        if (question == "quit" || question == "exit" || question == "q") {
            std::cout << "\nThe Magic 8 Ball bids you farewell!" << std::endl;
            break;
        }

        // Check if question is empty
        if (question.empty()) {
            std::cout << "Please ask a question!\n" << std::endl;
            continue;
        }

        // Generate random response
        int index = std::rand() % numResponses;

        std::cout << "\n*shakes the Magic 8 Ball*" << std::endl;
        std::cout << "The Magic 8 Ball says: " << responses[index] << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
