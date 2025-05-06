output_file="input2.bin"
count=103

for ((i = 0; i < count; i++)); do
  num=$((RANDOM % 1000))
  printf "%d\n" "$num" | od -An -td4 | awk '{ printf("%c%c%c%c", $1%256, ($1/256)%256, ($1/65536)%256, ($1/16777216)%256) }' >> "$output_file"
done

echo "Файл '$output_file' с '$count' числами создан."
