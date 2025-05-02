#ifndef DATABASE_H
#define DATABASE_H

#include <libpq-fe.h>
#include <stdio.h>

/**
 * @brief Gère les erreurs PostgreSQL et quitte le programme
 * @param conn Connexion à la base de données
 */
void exitOnError(PGconn *conn);

/**
 * @brief Crée la base de données principale
 * @param conn Connexion PostgreSQL
 */
void createDatabase(PGconn *conn);

/**
 * @brief Crée la table des rôles
 * @param conn Connexion PostgreSQL
 */
void createRolesTable(PGconn *conn);

/**
 * @brief Crée la table des utilisateurs
 * @param conn Connexion PostgreSQL
 * @note Inclut les champs :
 * - id (serial)
 * - username (varchar)
 * - email (varchar, unique)
 * - password (varchar)
 * - salt (varchar)
 * - role_id (foreign key vers roles)
 */
void createUserTable(PGconn *conn);

/**
 * @brief Crée la table des canaux
 * @param conn Connexion PostgreSQL
 * @note Inclut les champs :
 * - id (serial)
 * - name (varchar, unique)
 * - is_private (boolean)
 * - owner_id (foreign key vers users)
 */
void createChannelsTable(PGconn *conn);

/**
 * @brief Crée la table des membres des canaux
 * @param conn Connexion PostgreSQL
 * @note Relation many-to-many entre users et channels
 */
void createChannelMembersTable(PGconn *conn);

/**
 * @brief Crée la table des messages
 * @param conn Connexion PostgreSQL
 * @note Inclut les champs :
 * - channel_id (foreign key)
 * - sender_id (foreign key)
 * - content (text)
 * - timestamp (auto)
 * - is_private (boolean)
 * - encrypted (boolean)
 */
void createMessagesTable(PGconn *conn);

/**
 * @brief Crée la table des réactions aux messages
 * @param conn Connexion PostgreSQL
 */
void createReactionsTable(PGconn *conn);

/**
 * @brief Crée la table des messages privés
 * @param conn Connexion PostgreSQL
 * @note Structure similaire à messages mais pour les conversations privées
 */
void createPrivateMessagesTable(PGconn *conn);

#endif 