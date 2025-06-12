#!/bin/bash

set -e  # Останавливаем выполнение при ошибке

echo "Сборка в lab01/server..."
cd lab01/server && make && cd -

echo "Сборка в lab02..."
cd lab02 && make && cd -

echo "Сборка в lab03/server..."
cd lab03/server && make && cd -

echo "Сборка завершена!"
