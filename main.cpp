#include <iostream>
#include <limits>
#include <string>
#include <filesystem>

#include "library.h"
#include "filemanager.h"

using namespace std;

void clearScreen() {
    system("cls || clear");
}

void pauseForInput() {
    cout << "\nAppuyez sur Entrée pour continuer...";
    cin.ignore();
    cin.get();
}

void displayMenu() {
    cout << "\n=== SYSTÈME DE GESTION DE BIBLIOTHÈQUE PERSONNELLE ===\n";
    cout << "1.  Ajouter un Livre\n";
    cout << "2.  Supprimer un Livre\n";
    cout << "3.  Rechercher des Livres par Titre\n";
    cout << "4.  Rechercher des Livres par Auteur\n";
    cout << "5.  Afficher Tous les Livres\n";
    cout << "6.  Afficher les Livres Disponibles\n";
    cout << "7.  Ajouter un Utilisateur\n";
    cout << "8.  Afficher Tous les Utilisateurs\n";
    cout << "9.  Emprunter un Livre\n";
    cout << "10. Retourner un Livre\n";
    cout << "11. Statistiques de la Bibliothèque\n";
    cout << "12. Sauvegarder les Données\n";
    cout << "13. Créer une Sauvegarde\n";
    cout << "0.  Quitter\n";
    cout << "======================================================\n";
    cout << "Entrez votre choix : ";
}

string getInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

void printUsage(const char* programName) {
    cerr << "Usage: " << programName << " [--data-dir|-d <répertoire>]\n";
}

int main(int argc, char* argv[]) {
    string dataDir;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--data-dir" || arg == "-d") {
            if (i + 1 >= argc) {
                cerr << "Erreur : " << arg << " nécessite un répertoire.\n";
                printUsage(argv[0]);
                return 1;
            }
            dataDir = argv[++i];
        } else {
            cerr << "Erreur : argument inconnu '" << arg << "'.\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    string booksFile;
    string usersFile;
    string logFile = "journal.log";
    if (!dataDir.empty()) {
        if (!filesystem::is_directory(dataDir)) {
            cerr << "Erreur : le répertoire " << dataDir << " n'existe pas.\n";
            printUsage(argv[0]);
            return 1;
        }
        booksFile = (filesystem::path(dataDir) / "books.txt").string();
        usersFile = (filesystem::path(dataDir) / "users.txt").string();
        logFile = (filesystem::path(dataDir) / "journal.log").string();
    }

    Library library;
    FileManager fileManager(booksFile, usersFile, logFile);
    
    // Load existing data
    cout << "Chargement des données de la bibliothèque...\n";
    fileManager.loadLibraryData(library);
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        
        if (!(cin >> choice)) {
            cout << "Saisie invalide. Veuillez entrer un nombre.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pauseForInput();
            continue;
        }
        cin.ignore(); // Clear newline from buffer
        
        switch (choice) {
            case 1: { // Add Book
            // Détection des doublons plus intelligente
            // Verification de doublant pour : Ajouter livre (même titre, Auteur, ISBN)
                string title = getInput("Entrez le titre du livre : ");
                string author = getInput("Entrez l'auteur du livre : ");
                string isbn = getInput("Entrez l'ISBN du livre : ");
                
                if (library.doublonsPlusIntelligente(title, author, isbn)) {
                    cout << "Erreur : Un livre avec l'ISBN " << isbn << " existe déjà.\n";
                } else {
                    Book newBook(title, author, isbn);
                    library.addBook(newBook);
                    fileManager.logActivity("AJOUT LIVRE", newBook.toFileFormat());
                    cout << "Livre ajouté avec succès !\n";
                }
                pauseForInput();
                break;
            }
            
            case 2: { // Remove Book
                string isbn = getInput("Entrez l'ISBN du livre à supprimer : ");
                
                if (library.removeBook(isbn)) {
                    cout << "Livre supprimé avec succès !\n";
                    fileManager.logActivity("SUPPRESSION", "ISBN du livre supprimé : " + isbn);
                } else {
                    cout << "Livre non trouvé.\n";
                    fileManager.logActivity("SUPPRESSION", "Essai de suppression : " + isbn);
                }
                pauseForInput();
                break;
            }
            
            case 3: { // Search by Title
                string title = getInput("Entrez le titre à rechercher : ");
                auto results = library.searchBooksByTitle(title);
                
                if (results.empty()) {
                    cout << "Aucun livre trouvé avec ce titre.\n";
                    fileManager.logActivity("RECHERCHE", "Essai de recherche (titre) : " + title);
                } else {
                    cout << "\n=== RÉSULTATS DE RECHERCHE ===\n";
                    for (size_t i = 0; i < results.size(); ++i) {
                        cout << "\nRésultat " << (i + 1) << " :\n";
                        cout << results[i]->toString() << "\n";
                        cout << "-----------------------------\n";
                    }
                    fileManager.logActivity("RECHERCHE", "Titre recherché : " + title);
                }
                pauseForInput();
                break;
            }
            
            case 4: { // Search by Author
                string author = getInput("Entrez l'auteur à rechercher : ");
                auto results = library.searchBooksByAuthor(author);
                
                if (results.empty()) {
                    cout << "Aucun livre trouvé de cet auteur.\n";
                    fileManager.logActivity("RECHERCHE", "Essai de recherche (auteur) : " + author);
                } else {
                    cout << "\n=== RÉSULTATS DE RECHERCHE ===\n";
                    for (size_t i = 0; i < results.size(); ++i) {
                        cout << "\nRésultat " << (i + 1) << " :\n";
                        cout << results[i]->toString() << "\n";
                        cout << "-----------------------------\n";
                    }
                    fileManager.logActivity("RECHERCHE", "Auteur recherché : " + author);
                }
                pauseForInput();
                break;
            }
            
            case 5: // Display All Books
                library.displayAllBooks();
                pauseForInput();
                fileManager.logActivity("AFFICHAGE", "Tous les livres affichés");
                break;
            
            case 6: // Display Available Books
                library.displayAvailableBooks();
                pauseForInput();
                fileManager.logActivity("AFFICHAGE", "Livres disponibles affichés");
                break;
            
            case 7: { // Add User
                string name = getInput("Entrez le nom de l'utilisateur : ");
                string userId = getInput("Entrez l'ID de l'utilisateur : ");
                
                if (library.findUserById(userId)) {
                    cout << "Erreur : Un utilisateur avec l'ID " << userId << " existe déjà.\n";
                    fileManager.logActivity("AJOUT", "Essai d'ajout (ID utilisateur) : " + userId);
                } else {
                    User newUser(name, userId);
                    library.addUser(newUser);
                    cout << "Utilisateur ajouté avec succès !\n";
                    fileManager.logActivity("AJOUT", "Utilisateur ajouté : " + userId);
                }
                pauseForInput();
                break;
            }
            
            case 8: // Display All Users
                library.displayAllUsers();
                pauseForInput();
                fileManager.logActivity("AFFICHAGE", "Tous les utilisateurs affichés");
                break;
            
            case 9: { // Check Out Book
                string isbn = getInput("Entrez l'ISBN du livre à emprunter : ");
                string userId = getInput("Entrez l'ID de l'utilisateur : ");
                
                Book* book = library.findBookByISBN(isbn);
                User* user = library.findUserById(userId);
                string bookTitle = book ? book->getTitle() : "";
                string bookAuthor = book ? book->getAuthor() : "";
                string userName = user ? user->getName() : userId;

                if (library.checkOutBook(isbn, userId)) {
                    fileManager.logActivity("EMPRUNT", bookTitle + "|" + bookAuthor + "|" + isbn + "|0|" + userName);
                    cout << "Livre emprunté avec succès !\n";
                    fileManager.logActivity("EMPRUNT", "Livre emprunté : " + isbn + " par l'utilisateur : " + userId);

                } else {
                    cout << "Erreur : Impossible d'emprunter le livre. Vérifiez l'ISBN, l'ID utilisateur et la disponibilité du livre.\n";
                    fileManager.logActivity("EMPRUNT", "Essai d'emprunt échoué : " + isbn + " par l'utilisateur : " + userId);
                }
                pauseForInput();
                break;
            }
            
            case 10: { // Return Book
                string isbn = getInput("Entrez l'ISBN du livre à retourner : ");
                Book* book = library.findBookByISBN(isbn);
                string bookTitle = book ? book->getTitle() : "";
                string bookAuthor = book ? book->getAuthor() : "";
                
                if (library.returnBook(isbn)) {
                    fileManager.logActivity("RETOUR LIVRE", bookTitle + "|" + bookAuthor + "|" + isbn + "|1|");
                    cout << "Livre retourné avec succès !\n";
                    fileManager.logActivity("RETOUR LIVRE", "Livre retourné : " + isbn);

                } else {
                    cout << "Erreur : Impossible de retourner le livre. Vérifiez l'ISBN et que le livre est bien emprunté.\n";
                    fileManager.logActivity("RETOUR LIVRE", "Essai de retour échoué : " + isbn);
                }
                pauseForInput();
                break;
            }
            
            case 11: { // Library Statistics
                cout << "\n=== STATISTIQUES DE LA BIBLIOTHÈQUE ===\n";
                cout << "Total des Livres : " << library.getTotalBooks() << "\n";
                cout << "Livres Disponibles : " << library.getAvailableBookCount() << "\n";
                cout << "Livres Empruntés : " << library.getCheckedOutBookCount() << "\n";
                cout << "Total des Utilisateurs : " << library.getAllUsers().size() << "\n";
                fileManager.logActivity("STATISTIQUES", "Affichage des statistiques de la bibliothèque");
                pauseForInput();
                break;
            }
            
            case 12: { // Save Data
                if (fileManager.saveLibraryData(library)) {
                    cout << "Données de la bibliothèque sauvegardées avec succès !\n";
                    fileManager.logActivity("SAUVEGARDE", "Données de la bibliothèque sauvegardées avec succès");
                } else {
                    cout << "Erreur lors de la sauvegarde des données de la bibliothèque.\n";
                    fileManager.logActivity("SAUVEGARDE", "Erreur lors de la sauvegarde des données de la bibliothèque");
                }
                pauseForInput();
                break;
            }
            
            case 13: { // Create Backup
                fileManager.createBackup();
                fileManager.logActivity("SAUVEGARDE", "Sauvegarde créée");
                pauseForInput();
                break;
            }
            
            case 0: // Exit
                cout << "Sauvegarde des données avant la fermeture...\n";
                if (fileManager.saveLibraryData(library)) {
                    cout << "Données de la bibliothèque sauvegardées avec succès !\n";
                }
                cout << "Merci d'avoir utilisé le Système de Gestion de Bibliothèque Personnelle !\n";
                running = false;
                break;
            
            default:
                cout << "Choix invalide. Veuillez réessayer.\n";
                pauseForInput();
                break;
        }
    }
    
    return 0;
}