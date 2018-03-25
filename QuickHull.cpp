#include <GL\GL.h>
#include <GL\freeglut.h>
#include <bits/stdc++.h>
#include <time.h>

// Point adalah pasangan dari absis maupun ordinat
#define Point pair<int, int>

using namespace std;
 
// Menyimpan titik-titik yang digenerate secara random dan dipastikan bahwa banyaknya sama persis dengan inputan user
set<Point> PointSet;
// Menyimpan sisi-sisi yang menjadi kandidat solusi dari algoritma QuickHull
vector<pair<Point,Point>> HullCandidate;
int n,i,coun=0,k;
// Menyimpan semua kemungkinan dari titik yang ada agar dapat diakses lebih mudah karena pengaksesan array akan jauh lebih mudah dibandingkan perlu pengaksesan dari library set
Point p[25];
 
 
// Mengembalikan nilai dari perspektif arah dari sebuah titik terhadap sebuah garis, dengan nilai positif maka konveks ke atas sedangkan nilai negatif adalah konveks kebawah
int CariArah(Point p1, Point p2, Point p)
{
	int val = (p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first);
	if (val > 0)
		return 1;
	if (val < 0)
		return -1;
	return 0;
}
 
// Mengembalikan luas segitiga yang dapat dibentuk dari p1,p2,p
// mengingat luas segitiga sebanding atau berbanding lurus dengan jarak dari suatu titik x ke garis yang dibentuk oleh y dan z
double Jarak(Point x, Point y, Point z)
{
    return abs ((z.second - x.second) * (y.first - x.first) - (y.second - x.second) * (z.first - x.first));
}

// Prosedur yang akan direkursifkan dengan nilai dari p1 dan p2 sebagai batasan ujung dari konveks hull
// lalu nilai dari side yang menetukan apakah harus condong ke atas atau kearah bawah.
void quickHull(Point p[], int n, Point x, Point y, int side)
{
	int mid = -1;
	double max_Jarak = 0;

	// iterasi untuk mengecek:
	// orientasi titik sesuai dengan arahnya && jaraknya lebih besar dari max sebelumnya
	for (int i=0; i<n; i++)
	{
		double temp = Jarak(x, y, p[i]);
		if (CariArah(x, y, p[i]) == side && temp > max_Jarak)
		{
			mid = i;
			max_Jarak = temp;
		}
	}
    
	// Jika sudah tidak ada titik yang memenuhi krteria mempunyai arah konveks serta nilai jarak dari titik > 0
	// maka titik ujung yaitu p1 dan p2 dimasukkan kedalam hhimpunan penyelesaian
	if (mid == -1)
    {
		HullCandidate.push_back({x,y});
		PointSet.insert(x);
		PointSet.insert(y);
		coun++;
		return;
	}

	// Secara rekursif cari dua bagian yang dibatasi dengan titik a[mid]
	quickHull(p, n, p[mid], x, -CariArah(p[mid], x, y));
	quickHull(p, n, p[mid], y, -CariArah(p[mid], y, x));
}
 
void CetakQuickHull(Point p[], int n)
{

    // Cari titik maksimum dan titik minimum dari sumbu absis
	int min_x = 0, max_x = 0;
	for (int i=1; i<n; i++)
	{
		if (p[i].first < p[min_x].first)
			min_x = i;
		if (p[i].first > p[max_x].first)
			max_x = i;
	}
	
	// Secara rekursif mencari titik konvex hull yang mungkin dengan batasan a[min_x] dan a[max_x] pada kedua sisi
	quickHull(p, n, p[min_x], p[max_x], 1);
	quickHull(p, n, p[min_x], p[max_x], -1);
 
	cout << "The points in Convex Hull are:\n";
	cout<<"{";
	for (int i=0; i<coun; i++)
	{
		cout << "((" <<HullCandidate[i].first.first << ", " << HullCandidate[i].first.second << "),("<<HullCandidate[i].second.first << ", " << HullCandidate[i].second.second << "))";
		if(i!=coun-1) cout<<",";
	}
	puts("}");
}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2.5); 
	glColor3f(1.0, 0.0, 0.0);
	for(int i=0; i<coun; i++){
		glBegin(GL_LINES);
		glVertex3f(HullCandidate[i].first.first, HullCandidate[i].first.second, 0.0);
		glVertex3f(HullCandidate[i].second.first, HullCandidate[i].second.second, 0.0);
		glEnd();
	}
	glColor3f(0.0, 1.0, 0.0);
	for(int i=0; i<k; i++){
		glPointSize(2.5);
		glBegin(GL_POINTS);
		glVertex3f(p[i].first, p[i].second,0);
		glEnd();
	}	
	glBegin(GL_LINE_STRIP);
	glVertex2i(100, 0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(0, 100);
	glEnd();
	glFlush();
}

void Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}

void idle()
{
	glutPostRedisplay();
}

void Generator(int n){
	// Mengecek keadaan awal dari himpunan penampung titik-titik yang akan digenerate secara random
	while(!PointSet.empty()){
		PointSet.erase(PointSet.begin());
	}
	if(PointSet.empty()){
		// Selama jumlah titiknya masih kurang dari inputan request user, maka tinggal iterate sampai penuh
		while(PointSet.size()<n){
			int tempx = rand() % 100;
			int tempy = rand() % 100;
			Point temp = make_pair(tempx,tempy);
			PointSet.insert(temp);
		}
	}
}
 
// Main driver
int main(int iArgc, char** cppArgv)
{
	cout<<"Masukkan Jumlah Titik Yang Anda Inginkan:\n";
	cin>>k;
	Generator(k);
	int i=0;
	puts("");
	// Pemasukan nilai - nilai dari titik yang sudah digenerate secara random ke dalam array serta pencatatan keluar titik-titik yang tergenerate
	while(!PointSet.empty()){
		p[i].first = (*PointSet.begin()).first;
		p[i].second = (*PointSet.begin()).second;
		cout<<p[i].first<<" "<<p[i].second<<endl;
		PointSet.erase(PointSet.begin());
		i++;
	}
	// Penyetelan waktu dan mulai perhitungan waktu
	const clock_t begin_time = clock();
	// Pemanggilan fungsi inisialisasi dari QuickHull dan rekursif-rekursifnya
	CetakQuickHull(p, k);
	puts("");
	// Output waktu yang diperlukan dalam proses QuickHull
	cout<< float(clock()-begin_time) / CLOCKS_PER_SEC;
	// Insialisasi Penggambaran dengan OpenGL
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(250, 250);
	glutCreateWindow("Tugas Kecil 2 Strategi Algoritma");
	Initialize();
	glutDisplayFunc(Draw);
	glutIdleFunc(idle);
	glutMainLoop();
    return 0;
}