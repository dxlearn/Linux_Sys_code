/* file name: stacksize.c */
 
void *orig_stack_pointer;

 
void blow_stack() {
    blow_stack();
}
 
int main() {
    __asm__("movq %rsp,orig_stack_pointer");
    blow_stack();
    return 0;
}
//编译时要加-no-pie字段 https://www.cnblogs.com/sky-heaven/p/15953338.html