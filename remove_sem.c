#include <semaphore.h>
#include <stdio.h>

int main(void) {
	sem_unlink("/checkin_sem");
	sem_unlink("/checkout_sem");
	sem_unlink("/checkout_b");
	sem_unlink("/checkout_b");
}