

function directory {
    mkdir -p salida
}

function compile {
    if [[ $1 == "java" && -f "implementaciones/Tarea10.java" ]]; then
        echo "Compiling java.."
        rm -f salida/*.class
        javac implementaciones/Tarea10.java -d salida
    fi
    
    if [[ $1 == "cpp" && -f "implementaciones/Tarea10.cpp" ]]; then
        echo "Compiling cpp.."
        rm -f salida/*.exe
        g++ -o Tarea10 implementaciones/Tarea10.cpp
        if [ -f "Tarea10.exe" ]; then
            mv Tarea10.exe salida
        fi
    fi
}

function run {
    if [[ $1 == "java" && -f "salida/Tarea10.class" && -f "casos/$2.in" ]]; then
        echo "Running java..."
        cd salida
        java Tarea10 < ../casos/$2.in
        cd ..
    fi

    if [[ $1 == "python" && -f "implementaciones/Tarea10.py" && -f "casos/$2.in" ]]; then
        echo "Running python..."
        python implementaciones/Tarea10.py < casos/$2.in
    fi

    if [[ $1 == "cpp" && -f "salida/Tarea10.exe" && -f "casos/$2.in" ]]; then
        echo "Running cpp.."
        ./salida/Tarea10.exe < casos/$2.in
    fi
}


directory
compile $1  # $1 means "java", "python", "cpp"
run $1 $2   # $2 means test case identifier