count_bytes() {
    find . -maxdepth 1 -type f | while read -r file; do
        bytes=$(wc --bytes < "$file")
        echo "$bytes ${file#./}"
    done
}

count_all() {
    total_size=$(wc --bytes ./* 2>/dev/null | awk 'END{print $1}')
    echo "$total_size"
}

count_bytes
count_all