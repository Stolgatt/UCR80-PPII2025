#include "graphical_engine.h"
#include "engine_common.h"
#include <stdio.h>
//Fonctions de grphical_engine.c redéfinies ici pour les tests
inline void TRI_INSERTION(Z_SPRITE* tableau, unsigned int debut, unsigned int fin) {

	for (unsigned int i=debut+1; i<fin; ++i) {
		unsigned int j;
		Z_SPRITE tmp;
		j=debut;
		while (tableau[i].Z <= tableau[j].Z && j<i) j++;
		for (; j<i; ++j) {
			tmp = tableau[j];
			tableau[j] = tableau[i];
			tableau[i] = tmp;
		}
	}

}

inline void hopla(Z_SPRITE* tableau, unsigned int i, unsigned int j) {
	Z_SPRITE tmp = tableau[i];
	tableau[i] = tableau[j];
	tableau[j] = tmp;
}

// Compteur pour suivre le nombre de tests executés
int tests_executed = 0;

//Comparateur pour suivre le nombre de tests passés
int tests_passed = 0;

// Fonction pour incrémenter le compteur des tests executés
void test_executed() {
    tests_executed++;
}

//Fonction pour incrémenter le compteur des tests passés
void test_passed() {
    tests_passed++;
}

// Macro pour tester l'égalité
#define TEST_ASSERT_EQUAL(expected, actual) {\
    test_executed();\
    if ((expected) != (actual)) {\
        printf("Test number %d failed: %f expected but got %f\n",tests_executed, (expected), (actual));\
    }\
    else {\
        test_passed();\
    }\
}

void test_VECTEUR2D(void) {
    VECTEUR2D v1 = {3, 4};
    TEST_ASSERT_EQUAL(3, v1.x);
    TEST_ASSERT_EQUAL(4, v1.y);
}

void test_NORMALISER_VECTEUR2D(void) {
    VECTEUR2D v1 = {3, 4};
    VECTEUR2D v2 = {0,0};
    NORMALISER_VECTEUR2D(&v1, &v2);
    float a = 3.0/5.0;
    TEST_ASSERT_EQUAL(a, v2.x);
    float b = 4.0/5.0;
    TEST_ASSERT_EQUAL(b, v2.y);
}

void test_APPLIQUER_EN_PLACE_BASE_2D(void) {
    VECTEUR2D v1 = {3,4};
    VECTEUR2D i = {2,5};
    VECTEUR2D j = {3,6};
    APPLIQUER_EN_PLACE_BASE_2D(&v1,&i,&j);
    TEST_ASSERT_EQUAL(18, v1.x);
    TEST_ASSERT_EQUAL(39, v1.y);
}

void test_APPLIQUER_EN_PLACE_BASE_INVERSE_2D(void) {
    VECTEUR2D v1 = {1,3};
    VECTEUR2D i = {2,5};
    VECTEUR2D j = {3,6};
    APPLIQUER_EN_PLACE_BASE_INVERSE_2D(&v1,&i,&j);
    TEST_ASSERT_EQUAL(17, v1.x);
    TEST_ASSERT_EQUAL(21, v1.y);
}

void test_CALCUL_BASE_2D(void) {
    const float x =M_PI/4 ;
    const float y = 3;
    VECTEUR2D i = {2,3};
    VECTEUR2D j = {4,5};
    CALCUL_BASE_2D(&x,&y,&i,&j);
    float a = cos(x)*3;
    TEST_ASSERT_EQUAL(a, i.x);
    TEST_ASSERT_EQUAL(a, i.y);
    TEST_ASSERT_EQUAL(-a, j.x);
    TEST_ASSERT_EQUAL(a, j.y);
}

void test_OS2D(void){
    OS2D os;
    VECTEUR2D i = {2,3};
    VECTEUR2D j = {4,5};
    float x =M_PI/4 ;
    float y = 3;
    os.i = i;
    os.j = j;
    os.theta = x;
    os.echelle = y;
    CALCUL_BASE_OS2D_M(&os);
        float a = cos(x)*3;
    TEST_ASSERT_EQUAL(a, os.i.x);
    TEST_ASSERT_EQUAL(a, os.i.y);
    TEST_ASSERT_EQUAL(-a, os.j.x);
    TEST_ASSERT_EQUAL(a, os.j.y);
}

void test_INTERSECTION_SEGMENT2D(void){
    VECTEUR2D vectunit = {1,0};
    VECTEUR2D origine = {2,2};
    VECTEUR2D origin1 = {1,1};
    VECTEUR2D origin2 = {10,10};
    float longueur = 4;
    float r = 2;
    VECTEUR2D point = {0,0};
    SEGMENT2D seg = {vectunit, longueur};
    int resp=INTERSECTION_SEGMENT2D(&seg,&point,&r,&origine);
    TEST_ASSERT_EQUAL(1, resp);
    int resp1=INTERSECTION_SEGMENT2D(&seg,&point,&r,&origin1);
    TEST_ASSERT_EQUAL(1, resp1);
    int resp2=INTERSECTION_SEGMENT2D(&seg,&point,&r,&origin2);
    TEST_ASSERT_EQUAL(0, resp2);
}

void test_NORMALISER_QUATERNION(void){
    QUATERNION q = {1,2,3,4};
    QUATERNION q1 = {0,0,0,0};
    NORMALISER_QUATERNION(&q,&q1);
    float norm = 1.0/sqrt(30.0);
    float a = 1.0*norm;
    TEST_ASSERT_EQUAL(a, q1.w);
    float b = 2.0*norm;
    TEST_ASSERT_EQUAL(b, q1.x);
    float c = 3.0*norm;
    TEST_ASSERT_EQUAL(c, q1.y);
    float d = 4.0*norm;
    TEST_ASSERT_EQUAL(d, q1.z);
}

void test_MULT_QUATERNIONS_M(void){
    QUATERNION q1 = {1,2,3,4};
    QUATERNION q2 = {5,6,7,8};
    QUATERNION q3 = {0,0,0,0};
    MULT_QUATERNIONS_M(q1,q2,q3);
    TEST_ASSERT_EQUAL(-60, q3.w);
    TEST_ASSERT_EQUAL(12, q3.x);
    TEST_ASSERT_EQUAL(30, q3.y);
    TEST_ASSERT_EQUAL(24, q3.z);
}

void test_INVERSER_QUATERNIONS_M(void){
    QUATERNION q1 = {1,2,3,4};
    QUATERNION q2 = {0,0,0,0};
    INVERSER_QUATERNION_M(q1,q2);
    TEST_ASSERT_EQUAL(1, q2.w);
    TEST_ASSERT_EQUAL(-2, q2.x);
    TEST_ASSERT_EQUAL(-3, q2.y);
    TEST_ASSERT_EQUAL(-4, q2.z);

}

void test_MULT_QUATERNIONS_EN_PLACE_GAUCHE(void){
    QUATERNION q1 = {1,2,3,4};
    QUATERNION q2 = {5,6,7,8};
    MULT_QUATERNIONS_EN_PLACE_GAUCHE(&q1,&q2);
    TEST_ASSERT_EQUAL(-60, q1.w);
    TEST_ASSERT_EQUAL(12, q1.x);
    TEST_ASSERT_EQUAL(30, q1.y);
    TEST_ASSERT_EQUAL(24, q1.z);
}

void test_MULT_QUATERNIONS_EN_PLACE_DROITE(void){
    QUATERNION q1 = {1,2,3,4};
    QUATERNION q2 = {5,6,7,8};
    MULT_QUATERNIONS_EN_PLACE_DROITE(&q1,&q2);
    TEST_ASSERT_EQUAL(-60, q2.w);
    TEST_ASSERT_EQUAL(12, q2.x);
    TEST_ASSERT_EQUAL(30, q2.y);
    TEST_ASSERT_EQUAL(24, q2.z);
}

void test_CALCUL_QUATERNION(void){
    QUATERNION q1 = {1,2,3,4};
    float angle = M_PI/2;
    VECTEUR3D v = {5,6,7};
    CALCUL_QUATERNION(angle,&v,&q1);
    float a = cos(angle/2);
    TEST_ASSERT_EQUAL(a, q1.w);
    float b = 5*a;
    TEST_ASSERT_EQUAL(b, q1.x);
    float c = 6*a;
    TEST_ASSERT_EQUAL(c, q1.y);
    float d = 7*a;
    TEST_ASSERT_EQUAL(d, q1.z);
}

void test_TRI_INSERTION(void) {
    Z_SPRITE tab[5];
    tab[0].Z = 5;
    tab[1].Z = 4;
    tab[2].Z = 3;
    tab[3].Z = 2;
    tab[4].Z = 1;
    TRI_INSERTION(tab,0,4);
    TEST_ASSERT_EQUAL(5.0, tab[0].Z);
    TEST_ASSERT_EQUAL(4.0, tab[1].Z);
    TEST_ASSERT_EQUAL(3.0, tab[2].Z);
    TEST_ASSERT_EQUAL(2.0, tab[3].Z);
    TEST_ASSERT_EQUAL(1.0, tab[4].Z);
}

void test_hopla(void) {
    Z_SPRITE tab[5];
    tab[0].Z = 5;
    tab[1].Z = 4;
    tab[2].Z = 3;
    tab[3].Z = 2;
    tab[4].Z = 1;
    hopla(tab,0,4);
    TEST_ASSERT_EQUAL(1.0, tab[0].Z);
    TEST_ASSERT_EQUAL(5.0, tab[4].Z);
}

void test_TRI_RAPIDE(void) {
    Z_SPRITE tab[5];
    tab[0].Z = 5;
    tab[1].Z = 4;
    tab[2].Z = 3;
    tab[3].Z = 2;
    tab[4].Z = 1;
    TRI_RAPIDE(tab,0,4);
    TEST_ASSERT_EQUAL(5.0, tab[0].Z);
    TEST_ASSERT_EQUAL(4.0, tab[1].Z);
    TEST_ASSERT_EQUAL(3.0, tab[2].Z);
    TEST_ASSERT_EQUAL(2.0, tab[3].Z);
    TEST_ASSERT_EQUAL(1.0, tab[4].Z);
}



int main(void) {
    printf("Running tests...\n");
    test_VECTEUR2D();
    test_NORMALISER_VECTEUR2D();
    test_APPLIQUER_EN_PLACE_BASE_2D();
    test_APPLIQUER_EN_PLACE_BASE_INVERSE_2D();
    test_CALCUL_BASE_2D();
    test_OS2D();
    test_INTERSECTION_SEGMENT2D();
    test_NORMALISER_QUATERNION();
    test_MULT_QUATERNIONS_M();
    test_INVERSER_QUATERNIONS_M();
    test_MULT_QUATERNIONS_EN_PLACE_GAUCHE();
    test_MULT_QUATERNIONS_EN_PLACE_DROITE();
    test_CALCUL_QUATERNION();
    test_TRI_INSERTION();
    test_hopla();
    test_TRI_RAPIDE();
    printf("%d tests executed!\n", tests_executed);
    printf("%d tests passed!\n", tests_passed);
    return 0;
}
