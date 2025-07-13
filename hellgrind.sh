#!/bin/bash
# filepath: /home/eserraga/philo/test_helgrind.sh

echo "🧵 Testing philosophers with Helgrind..."

# Compilar
echo "📦 Compiling..."
make re

# Test cases
echo "🔍 Test 1: Basic case (3 philosophers)"
valgrind --tool=helgrind \
         --read-var-info=yes \
         --log-file=test1_helgrind.txt \
         ./philo 3 800 200 200

echo "📄 Results saved to test1_helgrind.txt"

echo "🔍 Test 2: Critical timing (4 philosophers)"
valgrind --tool=helgrind \
         --read-var-info=yes \
         --track-lockorders=yes \
         --log-file=test2_helgrind.txt \
         ./philo 4 410 200 100

echo "📄 Results saved to test2_helgrind.txt"

echo "🔍 Test 3: Many meals"
valgrind --tool=helgrind \
         --read-var-info=yes \
         --log-file=test3_helgrind.txt \
         ./philo 5 800 200 200 10

echo "📄 Results saved to test3_helgrind.txt"

echo "✅ All Helgrind tests completed!"
echo "Check the .txt files for detailed analysis."