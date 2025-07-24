#!/bin/bash
# test_fairness.sh

echo "🍽️ TESTING FAIRNESS ISSUE"

# Test con parámetros que deberían permitir supervivencia pero pueden causar starvation
echo "Testing: ./philo 3 650 200 100"
echo "Expected: All should survive, but some might starve due to unfair fork allocation"

# Ejecutar varias veces para ver patrones
for i in {1..5}; do
    echo -e "\n--- Test $i/5 ---"
    timeout 5s ./philo 3 650 200 100 2>&1 | grep "died\|is eating" | tail -10
    sleep 1
done

echo -e "\n🔍 Analysis:"
echo "- If you see the same philosopher eating repeatedly while others starve = UNFAIR"
echo "- If deaths occur despite sufficient time = STARVATION PROBLEM"
echo "- Philosophers should take turns more fairly based on hunger"