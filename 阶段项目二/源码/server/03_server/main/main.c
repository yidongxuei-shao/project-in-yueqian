#include "main.h"
Hnode_t *user_manager_hand;
theater_head_node_t *theater_manager_hand;


int main(void)
{
    /* 初始化用户数据 */
    /* 创建用户数据头节点 */
    user_manager_hand = DoubleLinkedList_Create();
    /* 加载用户数据 */
    /* 读取指定目录下的文件，并将数据加入到用户数据链表中 */
    loading_user_data(USER_DATA_DIR, user_manager_hand);
    DoubleLinkedList_PrintNode(user_manager_hand);
    /* 初始化剧场数据 */
    /* 创建剧场数据头节点 */
    printf("1\r\n");
    theater_manager_hand = DoubleLinkedList_Theater_Create();
    
    loading_theater_data(THEATER_DATA_DIR, theater_manager_hand);
    // DoubleLinkedList_Theater_PrintNode(theater_manager_hand);

    /*******************
     * 
     *      创建测试数据
     * 
     * *****************/
    
    DoubleLinkedList_Theater_PrintNode(theater_manager_hand);
    
    /* 创建线程，用于实时响应客户端的请求 */
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, server_start, NULL);

   
    int choice = 0;
    while(1)
    {
        // /* 管理剧场, 添加剧场的数据 */
        // printf("======================================\r\n");
        // printf("1.添加Actor剧场数据\r\n");
        // printf("2.添加Actor角色数据\r\n");
        scanf("%d", &choice);
        if(choice == 1)
        {
            DoubleLinkedList_PrintNode(user_manager_hand);
        }
        else if(choice == 2)
        {
            system("clear");
            /* 添加剧片 */
            printf("请输入要添加的剧名：");
            char name[30] = {0};
            scanf("%s", name);
            printf("name = %s\r\n", name);

             printf("请输入要添加的影票的价格：");
            char price[10] = {0};
            scanf("%s", price);
         
            printf("请输入影片的时长：");
            int duration = 0;
            scanf("%d", &duration);
            printf("请输入剧场编号：");
            int number = 0;
            scanf("%d", &number);
            theater_add_data(theater_manager_hand, 
                    name,
                    price,
                    "8:00",
                    duration,
                    number,
                    24);
        }
        else if(choice == 3)
        {
            /* 指定清空剧场座位数据 */
            printf("请输入要清空的影片：");
            char name[30] = {0};
            scanf("%s", name);

        }

    }

    return 0;
}