#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 进程结构体
 * */
typedef struct Pro{
    int id;             //进程号
    int in_time;        //到达时间
    int work_time;      //服务时间
    int btime;          //抢占式进程  开始时间
    int l_w_time;       //抢占式进程  剩余服务时间
    int end_time;       //进程结束时间
    int tab;            //标记
}Pro;


/**
 * 全局变量
 * */
int pro_num;            //进程数
Pro pro[10];            //进程

/**
 * 工具
 * */
/*进程初始化*/
void init(){
    printf("请输入进程数:");
    scanf("%d",&pro_num);
    printf("依次输入各进程的进程号，到达时间和服务时间:\n");
    for(int i=1;i<=pro_num;i++){
        scanf("%d%d%d",&pro[i].id,&pro[i].in_time,&pro[i].work_time);
    }
}

void print_top(char *s){
    printf("**********************%s**********************\n",s);
    printf("进程号 到达时间 服务时间 开始时间 结束时间 周转时间\n");
}

void print_end(){
    printf("*****************************************************\n");
}

void print(Pro pro,int begin){
    printf("%3d\t\t%3d\t\t%3d\t\t%3d\t\t%3d\t\t%3d\n",pro.id,pro.in_time,pro.work_time,
    begin,begin+pro.work_time,begin+pro.work_time-pro.in_time);
}

void printall(){
    for(int i=1;i<=pro_num;i++){
        printf("%3d\t\t%3d\t\t%3d\t\t%3d\t\t%3d\t\t%3d\n",pro[i].id,pro[i].in_time,pro[i].work_time,
        pro[i].btime,pro[i].end_time,pro[i].end_time-pro[i].in_time);
    }
}

void swap(Pro *p,Pro *q){
    int t_id,t_work_time,t_intime;
    t_id=p->id;t_intime=p->in_time;t_work_time=p->work_time;
    p->id=q->id;p->in_time=q->in_time;p->work_time=q->work_time;
    q->id=t_id;q->in_time=t_intime;q->work_time=t_work_time;
}

int find(int pp,int time){
    for(int i=1;i<=pro_num;i++){
        if(pro[pp].l_w_time==0 || (pro[i].l_w_time != 0 && pro[i].l_w_time<pro[pp].l_w_time && time>=pro[i].in_time)){
            pp=i;
        }
    }
    return pp;
}



/**
 * 先来先服务（FCFS）算法
 * */
void FCFS(){
    int i,j;
    int min;
    int t=pro_num;
    int begin=32767;

    for(i=1;i<=pro_num;i++){
        if(pro[i].in_time<begin)
            begin=pro[i].in_time;
        pro[i].tab=0;
    }                                   //找出进程开始时间并将全部进程标记位置0

    while (t--){
        for(i=1;i<=pro_num;i++){
            if(pro[i].tab==0){
                min=i;
                for(j=i+1;j<=pro_num;j++){
                    if(pro[j].tab==0 && pro[j].in_time<=pro[min].in_time)
                        min=j;
                }                       //找出所有进程中到达时间最小的
                pro[min].tab=1;
                print(pro[min],begin);
                begin+=pro[min].work_time;
            }
        }
    }
}

/**
 * 短作业优先（非抢占式）算法
 * */
void SJF_Nonp(){
    Pro *p,*q,*head;
    head=&pro[1];

    p=head;
    while (p-head<pro_num){
        for(q=p+1;q-head<pro_num;q++){
            if(q->in_time<p->in_time || (q->work_time<p->work_time && q->in_time==p->in_time)){
                swap(p,q);
            }
        }
        p++;
    }                                   //将全部进程以到达时间从小到大排序

    p=head;
    for(q=head;q<head+pro_num;q++){
        q->tab=0;
        /*if(q->in_time<p->in_time)
            p=q;
        if(q->in_time==p->in_time && q->work_time<p->work_time)
            p=q;*/
    }

    int time=0;
    int t=pro_num;
    p=head;
    while (t--){
        time=time < p->in_time ? p->in_time : time;
        p->tab=1;
        //p->begin_time=time;
        time+=p->work_time;
        p->end_time=time;
        for(q=head;q<head+pro_num;q++){
            if(p->tab==1 && q->tab==0)
                p=q;
            else if(p->tab==0 && (q->work_time<p->work_time))
                p=q;
        }
    }
    for(p=head;p<head+pro_num;p++){
        print(*p,p->end_time-p->work_time);
    }
}

/**
 * 短作业优先（抢占式）算法
 * */
void SJF_Preem(){
    Pro *p,*q,*head;
    head=&pro[1];
    int i;
    int time_cnt=0,time;

    p=head;
    while (p-head<pro_num){
        for(q=p+1;q-head<pro_num;q++){
            if(q->in_time<p->in_time){
                swap(p,q);
            }
        }
        p++;
    }

    for(i=1;i<=pro_num;i++){
        pro[i].l_w_time=pro[i].work_time;
        time_cnt+=pro[i].work_time;
    }

    int pp=1;
    time=pro[pp].in_time;

    while (time_cnt--){
        pro[pp].l_w_time--;
        time++;
        if(pro[pp].l_w_time==0){
            pro[pp].end_time=time;
        }
        if(pro[pp].btime==0 && pro[pp].in_time != 0){
            pro[pp].btime=time-1;
        }
        pp=find(pp,time);
    }
    printall();
}

/**
 * 时间片轮转
 * */
void RR(int qq){
    Pro *p,*q,*head;
    head=&pro[1];

    p=head;
    while (p-head<pro_num){
        for(q=p+1;q-head<pro_num;q++){
            if(q->in_time<p->in_time){
                swap(p,q);
            }
        }
        p++;
    }

    int time=pro[1].in_time;
    for(p=head;p<head+pro_num;p++){
        p->tab=0;
        p->l_w_time=p->work_time;
    }

    int flag=1,flag2,tmp;
    for(p=head;flag;p++){
        flag2=0;
        if(p->in_time<=time && p->l_w_time>0){
            if(p->l_w_time>=qq){
                p->l_w_time-=qq;
            } else{
                tmp=p->l_w_time;
                p->l_w_time=0;
                flag2=1;
            }
            if(p->tab == 0){
                p->tab=1;
                p->btime=time;
            }
            if(p->l_w_time == 0){
                if(flag2)
                    p->end_time=time+tmp;
                else
                    p->end_time=time+qq;
//                printf("end time:%d\n",p->end_time);
            }
        } else continue;

        if(flag2)
            time+=tmp;
        else
            time+=qq;
//        printf("time:%d\n",time);
        for(q=head;q<head+pro_num;q++){
            if(q->l_w_time != 0) break;
        }
        if(q==head+pro_num) flag=0;
        if(p==head+pro_num-1) p=head-1;
    }
        printall();
}


int main() {
/*    char s[] = {"先来先服务"};
    init();
    print_top(s);
    FCFS();
    print_end();*/

/*    char s[]={"短作业优先（非抢占式）"};
    init();
    print_top(s);
    SJF_Nonp();
    print_end();
    return 0;*/

/*    char s[]={"短作业优先（抢占式）"};
    init();
    print_top(s);
    SJF_Preem();
    print_end();
    return 0;*/

/*    char s[]={"时间片轮转"};
    init();
    print_top(s);
    RR(1);
    print_end();
    return 0;*/
    int d=0,qq;
    while (d!=EOF){
        printf("*****************操作系统课程设计--进程调度*****************\n");
        printf("请选择使用何种进程调度算法：\n");
        printf("1.先来先服务（FCFS）算法\n");
        printf("2.短作业优先（非抢占式）算法\n");
        printf("3.短作业优先（抢占式）算法\n");
        printf("4.轮转调度（RR）算法\n");
        scanf("%d",&d);
        switch (d){
            case 1:{
                char s[] = {"先来先服务"};
                init();
                print_top(s);
                FCFS();
                print_end();
                break;
            }
            case 2:{
                char s[]={"短作业优先（非抢占式）"};
                init();
                print_top(s);
                SJF_Nonp();
                print_end();
                break;
            }
            case 3:{
                char s[]={"短作业优先（抢占式）"};
                init();
                print_top(s);
                SJF_Preem();
                print_end();
                break;
            }
            case 4:{
                printf("请输入时间片大小：");
                scanf("%d",&qq);
                char s[]={"时间片轮转"};
                init();
                print_top(s);
                RR(qq);
                print_end();
                break;
            }
            default:{
                printf("输入有误！");
            }
        }
        getchar();
        system("clear");
    }
    return 0;
}