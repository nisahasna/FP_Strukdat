Project Simulasi Bola Memantul menggunakan Algoritma Bruteforce dan Quadtree

Program ini mensimulasikan pergerakan dan tumbukan banyak bola di dalam sebuah jendela dua dimensi menggunakan library Raylib. Program menyediakan dua metode pendeteksian tumbukan antar bola, yaitu algoritma Bruteforce dan Quadtree. Pada metode Bruteforce, setiap bola dibandingkan dengan seluruh bola lainnya untuk mendeteksi tumbukan, sehingga jumlah pengecekan meningkat seiring bertambahnya jumlah bola. Pada metode Quadtree, area simulasi dibagi menjadi beberapa bagian menggunakan struktur data quadtree sehingga pengecekan tumbukan hanya dilakukan pada bola-bola yang berada di area berdekatan, membuat proses lebih efisien.
Pengguna dapat mengganti mode pendeteksian tumbukan antara Bruteforce dan Quadtree dengan menekan tombol spasi. 

Tujuan Program
- Membandingkan performa dan cara kerja algoritma Bruteforce dan Quadtree
- Mengimplementasikan struktur data Quadtree untuk optimasi collision detection

Cara Kerja Program
Membuat window berukuran 1000×700. Menghasilkan sejumlah bola dengan posisi dan kecepatan acak. Setiap bola memiliki radius tetap dan kecepatan dua dimensi. Posisi bola diperbarui berdasarkan kecepatan dan delta time. Bola memantul jika menyentuh batas window (collision dengan dinding)

Program memiliki dua mode tumbukan:

1. Bruteforce
- Setiap bola dibandingkan dengan semua bola lainnya
- Kompleksitas: O(n²)
- Cocok untuk jumlah bola kecil

2. Quadtree
- Area window dibagi menjadi beberapa node Quadtree
- Bola hanya dicek tumbukannya dengan bola di area terdekat
- Lebih efisien untuk jumlah bola besar

Mode dapat diganti dengan menekan tombol SPACE.

Kesimpulan

Program simulasi bola memantul ini menunjukkan bahwa penggunaan algoritma pendeteksian tumbukan sangat berpengaruh terhadap efisiensi komputasi. Metode Bruteforce mudah diimplementasikan namun kurang efisien ketika jumlah bola meningkat karena harus melakukan pengecekan antar semua pasangan objek. Sebaliknya, metode Quadtree mampu mengoptimalkan proses pendeteksian tumbukan dengan membagi ruang simulasi sehingga pengecekan hanya dilakukan pada objek yang berdekatan. Dengan bantuan library Raylib, simulasi dapat divisualisasikan secara real-time dan jelas, sehingga program ini efektif digunakan sebagai media pembelajaran untuk memahami konsep collision detection dan struktur data spasial.
