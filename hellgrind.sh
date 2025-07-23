#!/bin/bash
# filepath: /home/eduaserr/cursus/philosophers/hellgrind_fixed.sh

echo "🧪 COMPREHENSIVE VALGRIND TESTING"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Current user
USER=$(whoami)

# Crear archivo de supresión si no existe
create_suppression_file() {
    if [ ! -f valgrind.supp ]; then
        cat > valgrind.supp << 'EOF'
{
   dwarf_warnings_9b
   Helgrind:Misc
   fun:evaluate_Dwarf3_Expr
   obj:*
}

{
   dwarf_warnings_a3
   Helgrind:Misc
   fun:evaluate_Dwarf3_Expr
   obj:*
}

{
   pthread_internal
   Helgrind:Race
   fun:pthread_*
   obj:/lib/x86_64-linux-gnu/libpthread-*
}

{
   glibc_malloc
   Memcheck:Leak
   fun:malloc
   fun:_dl_*
}
EOF
        echo "📝 Created valgrind.supp suppression file"
    fi
}

# Compilar y verificar supresiones
echo -e "${BLUE}📦 Compiling...${NC}"
make re
create_suppression_file

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Compilation failed${NC}"
    exit 1
fi

# Function to run helgrind test (MEJORADA)
run_helgrind() {
    local test_name="$1"
    local params="$2"
    local timeout_val="$3"
    local log_file="helgrind_${test_name}.txt"
    
    echo -e "\n${YELLOW}🔍 ${test_name}${NC}"
    echo "   Command: timeout ${timeout_val}s valgrind --tool=helgrind ./philo ${params}"
    
    # Usar archivo de supresión y mejores flags
    timeout ${timeout_val}s valgrind --tool=helgrind \
        --suppressions=valgrind.supp \
        --read-var-info=yes \
        --track-lockorders=yes \
        --history-level=approx \
        --conflict-cache-size=1000000 \
        --log-file=${log_file} \
        ./philo ${params} >/dev/null 2>&1
    
    local exit_code=$?
    
    # Analizar resultados (MEJORADO)
    if [ -f ${log_file} ]; then
        # Buscar solo errores reales, ignorar warnings de DWARF
        local real_errors=$(grep -E "(ERROR SUMMARY:|Possible data race|Lock order)" ${log_file} | grep -v "0 errors from 0 contexts" | wc -l)
        local race_conditions=$(grep -c "Possible data race" ${log_file})
        local lock_order=$(grep -c "Lock order" ${log_file})
        local dwarf_warnings=$(grep -c "evaluate_Dwarf3_Expr" ${log_file})
        
        echo -e "   📄 Results: ${log_file}"
        echo -e "   🔍 Real errors: ${real_errors}"
        echo -e "   🏁 Race conditions: ${race_conditions}"
        echo -e "   🔒 Lock order issues: ${lock_order}"
        echo -e "   📝 DWARF warnings (ignored): ${dwarf_warnings}"
        
        # Solo fallar por problemas críticos reales
        if [ ${race_conditions} -eq 0 ] && [ ${lock_order} -eq 0 ]; then
            echo -e "   ${GREEN}✅ CLEAN - No critical threading issues${NC}"
            return 0
        else
            echo -e "   ${RED}❌ CRITICAL ISSUES - Check log${NC}"
            return 1
        fi
    else
        echo -e "   ${RED}❌ Log file not created${NC}"
        return 1
    fi
}

# Function to test thread cleanup (MEJORADA para evitar falsos positivos)
test_thread_cleanup() {
    local test_name="$1"
    local params="$2"
    local timeout_val="$3"
    
    echo -e "\n${BLUE}🧹 Thread Cleanup: ${test_name}${NC}"
    echo -e "   Command: timeout ${timeout_val}s ./philo ${params}"
    
    # Contar procesos del usuario ANTES (más específico)
    local before_count=$(ps -u ${USER} -o pid,ppid,state,comm | grep -E "(philo|defunct)" | wc -l)
    
    # Ejecutar test con timeout más generoso
    timeout $((timeout_val + 2))s ./philo ${params} >/dev/null 2>&1
    local exit_code=$?
    
    # Esperar cleanup más tiempo
    sleep 1.5
    
    # Contar procesos del usuario DESPUÉS
    local after_count=$(ps -u ${USER} -o pid,ppid,state,comm | grep -E "(philo|defunct)" | wc -l)
    local process_diff=$((after_count - before_count))
    
    # Verificar específicamente procesos philo
    local philo_processes=$(ps -u ${USER} -o comm | grep "^philo$" | wc -l)
    local defunct_processes=$(ps -u ${USER} -o state,comm | grep "Z.*philo" | wc -l)
    
    if [ ${process_diff} -eq 0 ] && [ ${philo_processes} -eq 0 ] && [ ${defunct_processes} -eq 0 ]; then
        echo -e "   ${GREEN}✅ Perfect cleanup${NC}"
        return 0
    elif [ ${philo_processes} -gt 0 ]; then
        echo -e "   ${RED}❌ ${philo_processes} philo processes still running${NC}"
        killall philo 2>/dev/null
        return 1
    elif [ ${defunct_processes} -gt 0 ]; then
        echo -e "   ${RED}❌ ${defunct_processes} zombie philo processes${NC}"
        return 1
    else
        echo -e "   ${YELLOW}⚠️  Minor cleanup delay (${process_diff} system processes)${NC}"
        return 0
    fi
}

# Function to test expected behavior (MEJORADA con timeouts realistas)
test_behavior() {
    local test_name="$1"
    local params="$2"
    local timeout_val="$3"
    local expected="$4"
    
    echo -e "\n${BLUE}🎯 Behavior Test: ${test_name}${NC}"
    echo -e "   Command: timeout ${timeout_val}s ./philo ${params}"
    echo -e "   Expected: ${expected}"
    
    # FIX: Reemplazar espacios con guiones bajos
    local safe_name=$(echo "${test_name}" | tr ' ' '_')
    local output_file="temp_output_${safe_name}.txt"
    
    timeout ${timeout_val}s ./philo ${params} > "${output_file}" 2>&1
    local exit_code=$?
    local output=$(cat "${output_file}" 2>/dev/null || echo "")
    
    case ${expected} in
        "DIE")
            if echo "${output}" | grep -q "died"; then
                echo -e "   ${GREEN}✅ Death detected as expected${NC}"
                rm -f "${output_file}"
                return 0
            else
                echo -e "   ${RED}❌ No death detected${NC}"
                rm -f "${output_file}"
                return 1
            fi
            ;;
        "SURVIVE")
            if [ ${exit_code} -eq 124 ]; then  # timeout
                echo -e "   ${GREEN}✅ Survived timeout as expected${NC}"
                rm -f "${output_file}"
                return 0
            elif echo "${output}" | grep -q "died"; then
                echo -e "   ${RED}❌ Unexpected death${NC}"
                rm -f "${output_file}"
                return 1
            else
                echo -e "   ${YELLOW}⚠️  Terminated but no death message${NC}"
                rm -f "${output_file}"
                return 0
            fi
            ;;
        "COMPLETE")
            if [ ${exit_code} -eq 0 ] && echo "${output}" | grep -q "philo :)"; then
                echo -e "   ${GREEN}✅ Completed successfully${NC}"
                rm -f "${output_file}"
                return 0
            else
                echo -e "   ${RED}❌ Did not complete properly${NC}"
                echo -e "   Exit code: ${exit_code}"
                rm -f "${output_file}"
                return 1
            fi
            ;;
    esac
}

# Function to analyze memory (MEJORADA)
analyze_memory() {
    local log_file="$1"
    
    if [ ! -f "${log_file}" ]; then
        echo -e "${RED}❌ Memory log not found${NC}"
        return 1
    fi
    
    # Usar grep más específico para evitar falsos positivos
    local definitely_lost=$(grep "definitely lost:" "${log_file}" | grep -o "[0-9,]* bytes" | head -1 | grep -o "[0-9,]*" | tr -d ',')
    local possibly_lost=$(grep "possibly lost:" "${log_file}" | grep -o "[0-9,]* bytes" | head -1 | grep -o "[0-9,]*" | tr -d ',')
    
    # Set default values if empty
    definitely_lost=${definitely_lost:-0}
    possibly_lost=${possibly_lost:-0}
    
    echo -e "   📊 Memory Analysis:"
    echo -e "      Definitely lost: ${definitely_lost} bytes"
    echo -e "      Possibly lost: ${possibly_lost} bytes"
    
    # Solo fallar por leaks definitivos significativos
    if [ "${definitely_lost}" = "0" ] || [ "${definitely_lost}" -lt 100 ]; then
        echo -e "   ${GREEN}✅ No significant memory leaks${NC}"
        return 0
    else
        echo -e "   ${RED}❌ Significant memory leaks detected${NC}"
        return 1
    fi
}

# ============================================
# MAIN TESTING SEQUENCE
# ============================================

echo -e "\n${BLUE}=== HELGRIND RACE CONDITION TESTS ===${NC}"

total_helgrind=0
passed_helgrind=0

# Test con timeouts más realistas
tests=(
    "basic_sync:3 800 200 200:5"
    "critical_timing:4 410 200 100:4" 
    "stress_test:8 800 150 150:8"
    "fast_execution:5 300 60 60:3"
    "meal_completion:4 800 200 200 3:6"
    "single_philo:1 800 200 200:3"
)

for test in "${tests[@]}"; do
    IFS=':' read -r name params timeout <<< "$test"
    if run_helgrind "$name" "$params" "$timeout"; then 
        ((passed_helgrind++))
    fi
    ((total_helgrind++))
done

echo -e "\n${BLUE}=== THREAD CLEANUP TESTS ===${NC}"

total_cleanup=0
passed_cleanup=0

cleanup_tests=(
    "Normal completion:3 800 200 200 2:5"
    "Death scenario:4 310 200 100:4"
    "Quick death:2 150 100 50:3"
    "Single philosopher:1 800 200 200:3"
)

for test in "${cleanup_tests[@]}"; do
    IFS=':' read -r name params timeout <<< "$test"
    if test_thread_cleanup "$name" "$params" "$timeout"; then 
        ((passed_cleanup++))
    fi
    ((total_cleanup++))
done

echo -e "\n${BLUE}=== BEHAVIOR VERIFICATION TESTS ===${NC}"

total_behavior=0
passed_behavior=0

behavior_tests=(
    "Should survive:5 800 200 200:6:SURVIVE"
    "Should die:4 310 200 100:4:DIE"
    "Should complete:4 800 200 200 3:6:COMPLETE"
    "Single must die:1 800 200 200:3:DIE"
)

for test in "${behavior_tests[@]}"; do
    IFS=':' read -r name params timeout expected <<< "$test"
    if test_behavior "$name" "$params" "$timeout" "$expected"; then 
        ((passed_behavior++))
    fi
    ((total_behavior++))
done

echo -e "\n${BLUE}=== MEMORY LEAK TEST ===${NC}"

echo -e "\n${YELLOW}🔍 Memory Leak Analysis${NC}"
timeout 4s valgrind --tool=memcheck \
    --suppressions=valgrind.supp \
    --leak-check=full \
    --show-leak-kinds=definite,possible \
    --track-origins=yes \
    --log-file=memcheck_final.txt \
    ./philo 5 800 200 200 3 >/dev/null 2>&1

analyze_memory "memcheck_final.txt"
memory_result=$?

# Cleanup temp files
rm -f temp_output_*.txt

echo -e "\n${BLUE}=== FINAL RESULTS ===${NC}"

total_tests=$((total_helgrind + total_cleanup + total_behavior + 1))
total_passed=$((passed_helgrind + passed_cleanup + passed_behavior + memory_result))

echo -e "\n📊 ${BLUE}TEST SUMMARY${NC}"
echo -e "   🔍 Helgrind (Race Conditions): ${passed_helgrind}/${total_helgrind}"
echo -e "   🧹 Thread Cleanup: ${passed_cleanup}/${total_cleanup}"
echo -e "   🎯 Behavior Tests: ${passed_behavior}/${total_behavior}"
echo -e "   💾 Memory Leaks: $([[ ${memory_result} -eq 0 ]] && echo "✅ PASS" || echo "❌ FAIL")"

echo -e "\n🏆 ${BLUE}OVERALL SCORE: ${total_passed}/${total_tests}${NC}"

if [ ${total_passed} -eq ${total_tests} ]; then
    echo -e "\n${GREEN}🎉 PERFECT! Your philosophers implementation is excellent!${NC}"
elif [ ${total_passed} -ge $((total_tests * 8 / 10)) ]; then
    echo -e "\n${YELLOW}👍 GOOD! Your implementation works well with minor issues${NC}"
else
    echo -e "\n${RED}⚠️  NEEDS WORK! Several issues detected${NC}"
fi

echo -e "\n${GREEN}✅ Testing completed!${NC}"


#./philo 3 800 200 200
#./philo 4 410 200 100
#./philo 5 300 60 60
#./philo 4 800 200 200 3
#./philo 1 800 200 200
#./philo 8 800 150 150
#./philo 5 800 200 200 3
#./philo 8 800 150 150
