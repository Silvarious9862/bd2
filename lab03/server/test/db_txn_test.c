#include <db.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int ret;
    DB_ENV *env = NULL;
    DB *dbp = NULL;
    DB_TXN *txn = NULL;

    // Создаем окружение Berkeley DB.
    ret = db_env_create(&env, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка создания окружения: %s\n", db_strerror(ret));
        return EXIT_FAILURE;
    }
    
    // Открываем окружение с поддержкой транзакций.
    ret = env->open(env, "/home/silvarious/bd2/lab02/db_env", DB_CREATE | DB_INIT_MPOOL | DB_INIT_TXN, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка открытия окружения: %s\n", db_strerror(ret));
        env->close(env, 0);
        return EXIT_FAILURE;
    }
    
    // Создаем объект базы данных.
    ret = db_create(&dbp, env, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка создания объекта DB: %s\n", db_strerror(ret));
        env->close(env, 0);
        return EXIT_FAILURE;
    }
    
    // Открываем базу данных. Если базы еще нет, она будет создана.
    ret = dbp->open(dbp, NULL, "transfer_db.db", NULL, DB_BTREE, DB_CREATE, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка открытия базы данных: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return EXIT_FAILURE;
    }
    
    // Пытаемся начать транзакцию.
    ret = env->txn_begin(env, NULL, &txn, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка начала транзакции: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return EXIT_FAILURE;
    }
    
    printf("Транзакция успешно начата!\n");
    
    // Здесь можно выполнить необходимые операции в рамках транзакции.
    // Для теста просто коммитим транзакцию.
    ret = txn->commit(txn, 0);
    if (ret != 0) {
        fprintf(stderr, "Ошибка коммита транзакции: %s\n", db_strerror(ret));
        dbp->close(dbp, 0);
        env->close(env, 0);
        return EXIT_FAILURE;
    }
    
    printf("Транзакция успешно закоммичена!\n");
    
    // Закрываем базу и окружение.
    dbp->close(dbp, 0);
    env->close(env, 0);
    
    return EXIT_SUCCESS;
}
