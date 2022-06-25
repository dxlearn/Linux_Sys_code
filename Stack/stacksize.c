/* file name: stacksize.c */
 
void *orig_stack_pointer;

 
void blow_stack() {
    blow_stack();
}
 
int main() {
    register void *pvEbp __asm__ ("%esp");
    orig_stack_pointer = pvEbp;
    blow_stack();
    return 0;
}