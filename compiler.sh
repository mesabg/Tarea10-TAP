

function directory {
    mkdir -p salida
}

function compile {
    if [ $1 == "java" ]; then
        echo "Compiling java.."
        javac implementaciones/Tarea10.java -d salida
    fi
    
    if [ $1 == "cpp" ]; then
        echo "Compiling cpp.."
        g++ implementaciones/Tarea10.cpp -o Tarea10
        if [ -f "implementaciones/Tarea10.exe" ]; then
            mv implementaciones/Tarea10.exe salida
        fi
    fi
}

function run {
    if [ $1 == "java" ]; then
        echo "Running java..."
        cd salida
        java Tarea10 < ../casos/$2.in
        cd ..
    fi

    if [ $1 == "python" ]; then
        echo "Running python..."
        python implementaciones/Tarea10.py < casos/$2.in
    fi

    if [ $1 == "cpp" ]; then
        echo "Running cpp.."
        g++ implementaciones/Tarea10.cpp -o Tarea10
        if [ -f "implementaciones/Tarea10.exe" ]; then
            mv implementaciones/Tarea10.exe salida
            ./salida/Tarea10.exe < casos/$2.in
        fi
    fi
}


directory
compile $1  # $1 means "java", "python", "cpp"
run $1 $2   # $2 means test case identifier