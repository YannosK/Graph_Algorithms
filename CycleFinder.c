// FINDING CYCLES IN A GRAPH
#include <stdio.h>
#include <stdlib.h>

typedef struct node *node_pointer;
struct node
{
    int row;
    int column;
    node_pointer next;
    node_pointer back;
    node_pointer down;
    node_pointer up;

    unsigned short color; // 0:white , 1:grey , 2:black
    unsigned int distance;
    node_pointer parent;
    unsigned short leaf; // 0 if not leaf, 1 if leaf, 2 if an error happened
};

int white = 0, grey = 0, black = 0;

void insert(node_pointer r_h[], node_pointer c_h[], int r, int c);
void del(node_pointer r_h[], node_pointer c_h[], int r, int c);
void node(node_pointer r_h[]);
void print_row(node_pointer r_h[]);
void print_column(node_pointer c_h[]);
void bfs(node_pointer r_h[], int r);
void cyclefinder(node_pointer r_h[]);

int main(void)
{
    node_pointer row_head[30];
    node_pointer column_head[30];

    for (int i = 0; i < 30; i++)
        row_head[i] = NULL;
    for (int i = 0; i < 30; i++)
        column_head[i] = NULL;

    int row_data, column_data;

    char user_input;
    user_input = 'a';

    while (user_input != 'q')
    {
        fflush(stdin);
        printf("\n\n*******************************************************************************\n");
        printf("Previous choice %c\n", user_input);
        printf("Select what would you like to do:\n");
        printf("q : quit\ni : insert new edge using node data\nd : delete edge using node data\nr : prints all data of a row in the sparse array of edges\nc : prints all data of a column in the sparse array of edges\nn : shows the connections of an edge in the sparse array\nb : runs Breadth-First-Search algorithm on the graph\n");
        printf("Your choice: ");
        scanf("%c", &user_input);
        getchar();

        switch (user_input)
        {
        case 'q':
            printf("\n\n\tQUIT\n\n");
            break;
        case 'i':
            printf("\n\n\tINSERT\n\n");
            printf("\tInsert edge data\n\tInsert the two nodes as 'row' and 'column'\n\tRow: ");
            scanf("%d", &row_data);
            getchar();
            printf("\tColumn: ");
            scanf("%d", &column_data);
            getchar();
            insert(row_head, column_head, row_data, column_data);
            insert(row_head, column_head, column_data, row_data);
            break;
        case 'd':
            printf("\n\n\tDELETE\n\n");
            printf("\tInsert the data of the edge you wish to delete\n\tRow: ");
            scanf("%d", &row_data);
            getchar();
            if (row_head[row_data - 1] == NULL)
                printf("\tNo such edge was found\n\n");
            else
            {
                printf("\tColumn: ");
                scanf("%d", &column_data);
                getchar();
                if (column_head[column_data - 1] == NULL)
                    printf("\tNo such edge was found\n\n");
                else
                {
                    printf("\n");
                    del(row_head, column_head, row_data, column_data);
                    del(row_head, column_head, column_data, row_data);
                }
            }
            break;
        case 'r':
            printf("\n\n\tPRINT ROW DATA\n\n");
            print_row(row_head);
            break;
        case 'c':
            printf("\n\n\tPRINT COLUMN DATA\n\n");
            print_column(column_head);
            break;
        case 'n':
            printf("\n\n\tEDGE SPARSE ARRAY CONNECTIONS\n\n");
            node(row_head);
            break;
        case 'b':
            printf("\n\n\tBREADTH-FIRST SEARCH\n\n");
            printf("\tInsert starting-point node: ");
            scanf("%d", &row_data);
            getchar();
            if (row_head[row_data - 1] == NULL)
                printf("\tNo such node was found\n\n");
            else
                bfs(row_head, row_data);
            break;
        default:
            break;
        }
    }
    return 0;
}

void insert(node_pointer r_h[], node_pointer c_h[], int r, int c)
{
    node_pointer new_node, aux;

    new_node = (node_pointer)malloc(sizeof(struct node));

    int i = r - 1;
    int j = c - 1;

    new_node->column = c;
    new_node->row = r;
    new_node->next = NULL;
    new_node->back = NULL;
    new_node->down = NULL;
    new_node->up = NULL;
    new_node->color = 0;
    new_node->distance = 32767;
    new_node->parent = NULL;
    new_node->leaf = 2;

    // INSERT IN ROW
    if (r_h[i] != NULL)
    {
        if (new_node->column > r_h[i]->column)
        {
            aux = r_h[i];

            while ((aux->next != NULL) && (aux->next->column < new_node->column))
                aux = aux->next;

            if (aux->next != NULL)
            {
                if (aux->next->column == new_node->column)
                {
                    printf("\tInsertion not allowed. Edge already exists\n");
                    return;
                }
                else
                {
                    new_node->next = aux->next;
                    new_node->back = aux;
                    aux->next = new_node;
                }
            }
            else
            {
                new_node->next = aux->next;
                new_node->back = aux;
                aux->next = new_node;
            }
        }
        else if (new_node->column < r_h[i]->column)
        {
            new_node->next = r_h[i];
            r_h[i]->back = new_node;
            r_h[i] = new_node;
        }
        else if (new_node->column == r_h[i]->column)
        {
            printf("\tInsertion not allowed. Edge already exists\n");
            return;
        }
        else
            exit(1);
    }
    else if (r_h[i] == NULL)
        r_h[i] = new_node;
    else
        exit(1);

    // INSERT IN COLUMN
    if (c_h[j] != NULL)
    {
        if (new_node->row > c_h[j]->row)
        {
            aux = c_h[j];

            while ((aux->down != NULL) && (aux->down->row < new_node->row))
                aux = aux->down;

            if (aux->down != NULL)
            {
                if (aux->down->row == new_node->row)
                {
                    printf("\tInsertion not allowed. Edge already exists\n\n");
                    exit(1); // it has to terminate because something was inserted
                }
                else
                {
                    new_node->down = aux->down;
                    new_node->up = aux;
                    aux->down = new_node;
                }
            }
            else
            {
                new_node->down = aux->down;
                new_node->up = aux;
                aux->down = new_node;
            }
        }
        else if (new_node->row < c_h[j]->row)
        {
            new_node->down = c_h[j];
            c_h[j]->up = new_node;
            c_h[j] = new_node;
        }
        else if (new_node->row == c_h[j]->row)
        {
            printf("\tInsertion not allowed. Edge already exists\n\n");
            exit(1);
        }
        else
            exit(1);
    }
    else if (c_h[j] == NULL)
        c_h[j] = new_node;
    else
        exit(1);
}

void del(node_pointer r_h[], node_pointer c_h[], int r, int c)
{
    node_pointer node_to_delete, c_aux, c_aux2, r_aux, r_aux2;

    int i = r - 1;
    int j = c - 1;

    c_aux = r_h[i];
    r_aux = c_h[j];

    // BREAK ROW LIST CONNECTIONS
    if (c < r_h[i]->column)
    {
        printf("\tNo such node was found\n");
        return;
    }
    else if (c > r_h[i]->column)
    {
        // this catches the case that there is only the head and someone inputs something larger than it
        if (c_aux->next == NULL)
        {
            printf("\tNo such node was found\n");
            return;
        }

        while (c_aux->next != NULL && c_aux->next->column < c)
        {
            c_aux = c_aux->next;

            if (c_aux->next == NULL)
            {
                printf("\tNo such node was found\n");
                return;
            }
        }

        if (c_aux->next->column == c)
        {
            node_to_delete = c_aux->next;
            if (node_to_delete->next != NULL)
            {
                c_aux2 = node_to_delete->next;
                c_aux->next = c_aux2;
                c_aux2->back = c_aux;
                printf("\tBack edge: %d.%d\n\tNext edge: %d.%d\n", c_aux->row, c_aux->column, c_aux2->row, c_aux2->column);
            }
            else
            {
                c_aux->next = node_to_delete->next; // could as well be c_aux->next = NULL
                printf("\tBack edge: %d.%d\n\tNext edge: NULL\n", c_aux->row, c_aux->column);
            }
        }
        else
        {
            printf("\tNo such node was found\n");
            return;
        }
    }
    else if (c == r_h[i]->column && r_h[i]->next != NULL)
    {
        node_to_delete = c_aux;
        r_h[i] = c_aux->next;
        r_h[i]->back = NULL;
        printf("\tBack edge: NULL\n\tNext edge: %d.%d\n", r_h[i]->row, r_h[i]->column);
    }
    else if (c == r_h[i]->column && r_h[i]->next == NULL)
    {
        node_to_delete = c_aux;
        r_h[i] = NULL;
        printf("\tBack edge: NULL\n\tNext edge: NULL\n");
    }
    else
    {
        printf("\tFORBIDDEN\n");
        exit(2);
    }

    // BREAK COLUMN LIST CONNECTIONS
    if (r < c_h[j]->row)
    {
        printf("\tFORBIDDEN\n");
        exit(2);
    }
    else if (r > c_h[j]->row)
    {
        if (r_aux->down == NULL)
        {
            printf("\tFORBIDDEN\n");
            exit(2);
        }

        while (r_aux->down != NULL && r_aux->down->row < r)
        {
            r_aux = r_aux->down;

            if (r_aux->down == NULL)
            {
                printf("\tFORBIDDEN\n");
                exit(2);
            }
        }

        if (r_aux->down->row == r)
        {
            node_to_delete = r_aux->down;
            if (node_to_delete->down != NULL)
            {
                r_aux2 = node_to_delete->down;
                r_aux->down = r_aux2;
                r_aux2->up = r_aux;
                printf("\tUp edge: %d.%d\n\tDown edge: %d.%d\n", r_aux->row, r_aux->column, r_aux2->row, r_aux2->column);
            }
            else
            {
                r_aux->down = node_to_delete->down; // could as well be r_aux->down = NULL
                printf("\tUp edge: %d.%d\n\tDown edge: NULL\n", r_aux->row, r_aux->column);
            }
        }
        else
        {
            printf("\tFORBIDDEN\n");
            exit(2);
        }
    }
    else if (r == c_h[j]->row && c_h[j]->down != NULL)
    {
        node_to_delete = r_aux;
        c_h[j] = r_aux->down;
        c_h[j]->up = NULL;
        printf("\tUp edge: NULL\n\tDown edge: %d.%d\n", c_h[j]->row, c_h[j]->column);
    }
    else if (r == c_h[j]->row && c_h[j]->down == NULL)
    {
        node_to_delete = r_aux;
        c_h[j] = NULL;
        printf("\tUp edge: NULL\n\tDown edge: NULL\n");
    }
    else
    {
        printf("\tFORBIDDEN\n");
        exit(2);
    }

    printf("\tEdge to delete: %d.%d\n\n", node_to_delete->row, node_to_delete->column);
    free(node_to_delete);
}

void node(node_pointer r_h[])
{
    node_pointer aux;
    int row_data, column_data;
    printf("\tInsert the data of the edge you wish to see its connections\n\tRow: ");
    scanf("%d", &row_data);
    getchar();
    int i = row_data - 1;
    aux = r_h[i];

    if (r_h[i] == NULL)
    {
        printf("\tNo such node was found\n\n");
    }
    else
    {
        printf("\tColumn: ");
        scanf("%d", &column_data);
        getchar();

        if (column_data < r_h[i]->column)
        {
            printf("\tNo such node was found\n\n");
        }

        if (column_data > r_h[i]->column)
        {
            while (aux->column < column_data && aux->next != NULL)
            {
                aux = aux->next;
            }

            if (aux->column == column_data)
            {
                printf("\n\tRow: %d", aux->row);
                printf("\n\tColumn: %d", aux->column);

                if (aux->next != NULL)
                    printf("\n\tNext: %d.%d", aux->next->row, aux->next->column);
                else if (aux->next == NULL)
                    printf("\n\tNext: NULL");
                else
                    printf("NEXT IS NOT CONNECTED");

                if (aux->back != NULL)
                    printf("\n\tBack: %d.%d", aux->back->row, aux->back->column);
                else if (aux->back == NULL)
                    printf("\n\tBack: NULL");
                else
                    printf("BACK IS NOT CONNECTED");

                if (aux->up != NULL)
                    printf("\n\tUp: %d.%d", aux->up->row, aux->up->column);
                else if (aux->up == NULL)
                    printf("\n\tUp: NULL");
                else
                    printf("UP IS NOT CONNECTED");

                if (aux->down != NULL)
                    printf("\n\tDown: %d.%d\n\n", aux->down->row, aux->down->column);
                else if (aux->down == NULL)
                    printf("\n\tDown: NULL\n\n");
                else
                    printf("DOWN IS NOT CONNECTED\n\n");
            }
            else
                printf("\tNo such edge was found\n\n");
        }

        if (column_data == r_h[i]->column)
        {
            printf("\n\tYou added the head");
            printf("\n\tRow: %d", aux->row);
            printf("\n\tColumn: %d", aux->column);

            if (aux->next != NULL)
                printf("\n\tNext: %d.%d", aux->next->row, aux->next->column);
            else if (aux->next == NULL)
                printf("\n\tNext: NULL");
            else
                printf("NEXT IS NOT CONNECTED");

            if (aux->back != NULL)
                printf("\n\tBack: %d.%d", aux->back->row, aux->back->column);
            else if (aux->back == NULL)
                printf("\n\tBack: NULL");
            else
                printf("BACK IS NOT CONNECTED");

            if (aux->up != NULL)
                printf("\n\tUp: %d.%d", aux->up->row, aux->up->column);
            else if (aux->up == NULL)
                printf("\n\tUp: NULL");
            else
                printf("UP IS NOT CONNECTED");

            if (aux->down != NULL)
                printf("\n\tDown: %d.%d\n\n", aux->down->row, aux->down->column);
            else if (aux->down == NULL)
                printf("\n\tDown: NULL\n\n");
            else
                printf("DOWN IS NOT CONNECTED\n\n");
        }
    }
}

void print_row(node_pointer r_h[])
{
    node_pointer aux;
    int row_data, i;

    printf("\tInsert the row whose elements you want printed: ");
    scanf("%d", &row_data);
    getchar();

    i = row_data - 1;

    aux = r_h[i];

    if (aux == NULL)
        printf("\tThe list is empty\n");
    else
    {
        while (aux != NULL)
        {
            printf("\t%d\n", aux->column);
            aux = aux->next;
        }
    }
    printf("\n");
}

void print_column(node_pointer c_h[])
{
    node_pointer aux;
    int column_data, i;

    printf("\tInsert the column whose elements you want printed: ");
    scanf("%d", &column_data);
    getchar();

    i = column_data - 1;

    aux = c_h[i];

    if (aux == NULL)
    {
        printf("\tThe list is empty\n");
    }
    else
    {
        while (aux != NULL)
        {
            printf("\t%d\n", aux->row);
            aux = aux->down;
        }
    }
    printf("\n");
}

void bfs(node_pointer r_h[], int r)
{
    int i = r - 1;
    int j;
    int neighbor;
    unsigned int l_flag;
    node_pointer u, aux, parent;

    typedef struct vertex *vp;
    struct vertex
    {
        int v;
        vp n;
        vp b;
    };

    vp head = NULL;
    vp tail = NULL;
    vp temp, new_vertex;

    printf("\n\tParent of all nodes will be node %d\n", r_h[i]->row);
    white = 0;
    grey = 0;
    black = 0;

    for (j = 0; j < 30; j++)
    {
        if (r_h[j] != NULL)
        {
            r_h[j]->color = 0;
            r_h[j]->distance = 32767;
            r_h[j]->parent = NULL;
            r_h[j]->leaf = 2;
            white++;
        }
    }

    r_h[i]->color = 1;
    white--;
    grey++;
    r_h[i]->distance = 0;
    r_h[i]->parent = NULL;
    r_h[i]->leaf = 0;
    printf("\nInitial states:\nwhite = %d\ngrey = %d\nblack = %d\n\n", white, grey, black);

    // ENQUEUE SOURCE NODE
    head = (vp)malloc(sizeof(struct vertex));
    head->v = r_h[i]->row;
    head->n = NULL;
    head->b = NULL;
    tail = head;
    printf("Head is %d\n", head->v);

    while (head != NULL)
    {
        parent = r_h[tail->v - 1];
        aux = parent;
        printf("\tGrey one is %d.%d\n", parent->row, parent->column);

        // DEQUEUE THE TAIL
        temp = tail;
        if (temp->b == NULL)
        {
            head = NULL;
            tail = NULL;
            free(temp);
        }
        else
        {
            tail = tail->b;
            tail->n = NULL;
            free(temp);
        }

        l_flag = 1;
        while (aux != NULL)
        {
            neighbor = aux->column - 1;
            printf("\t\tNeighbor is node %d", neighbor + 1);

            if (r_h[neighbor] == NULL)
            {
                printf("and it is not connected!\n");
                exit(3);
            }
            else if (r_h[neighbor]->color == 2)
                printf(" and it is black\n");
            else if (r_h[neighbor]->color == 1)
                printf(" and it is already grey (in the queue)\n");
            else if (r_h[neighbor]->color == 0)
            {
                l_flag = 0;
                r_h[neighbor]->color = 1;
                white--;
                grey++;
                r_h[neighbor]->distance = parent->distance + 1;
                r_h[neighbor]->parent = parent;
                printf(" and it just turned grey (color %d)\n\t\tDistance: %d\n\t\tParent: %d\n", r_h[neighbor]->color, r_h[neighbor]->distance, r_h[neighbor]->parent->row);

                // ENQUEUE THE GREY
                new_vertex = (vp)malloc(sizeof(struct vertex));
                new_vertex->v = r_h[neighbor]->row;
                if (head != NULL)
                {
                    head->b = new_vertex;
                    new_vertex->n = head;
                    new_vertex->b = NULL;
                    head = new_vertex;
                }
                else
                {
                    head = new_vertex;
                    head->n = NULL;
                    head->b = NULL;
                    tail = head;
                }
            }
            printf("\t\t\tColors after neighbor %d:\n\t\t\twhite = %d\n\t\t\tgrey = %d\n\t\t\tblack = %d\n", neighbor + 1, white, grey, black);

            aux = aux->next;
        }
        if (l_flag == 0)
            parent->leaf = 0; // not a leaf
        else if (l_flag == 1)
            parent->leaf = 1; // a leaf
        else
            parent->leaf = 2; // undefined - error
        parent->color = 2;
        grey--;
        black++;

        printf("\n\tColors after checking all the neighbors of %d.%d\n\twhite = %d\n\tgrey = %d\n\tblack = %d\n\n", parent->row, parent->column, white, grey, black);
    }
    printf("\nColors after checking the entire array of nodes:\nwhite = %d\ngrey = %d\nblack = %d\n\n", white, grey, black);

    printf("\n\tBFS RESULTS:\n\n");
    for (j = 0; j < 30; j++)
    {
        if (r_h[j] != NULL)
        {
            printf("\tNode: %d\n\tDistance: %d\n", r_h[j]->row, r_h[j]->distance);
            printf("\tParent: ");
            if (r_h[j]->parent == NULL)
                printf(" itself (starting node)\n");
            else
                printf("%d\n", r_h[j]->parent->row);
            printf("\tColor: ");
            switch (r_h[j]->color)
            {
            case 0:
                printf("white\n");
                break;
            case 1:
                printf("grey\n");
                break;
            case 2:
                printf("black\n");
                break;
            default:
                break;
            }
            printf("\tIs ");
            switch (r_h[j]->leaf)
            {
            case 0:
                printf("not a leaf\n\n");
                break;
            case 1:
                printf("a leaf\n\n");
                break;
            case 2:
                printf("it a leaf? (ERROR)\n\n");
                break;
            default:
                break;
            }
        }
    }

    char answer;
    printf("\tDo you want to run cycle finder algorithm?\n\tIf you don't the breadth-first search results will be lost after this routine is over\n\t(press 'y' for yes, or 'n' for no): ");
    scanf("%c", &answer);
    getchar();
    if (answer == 'y')
        cyclefinder(r_h);
    else
        return;
}

void cyclefinder(node_pointer r_h[])
{
    int row_data, column_data, r, c;
    node_pointer aux;
    char a;

    printf("\n\n\tCYCLE FINDER\n\n");
    printf("\tAdd the two nodes that you want to find a cycle they belong in\n\tNote that they should be connected, and their edge should not belong in the bfs spanning tree\n");
    printf("\tRow: ");
    scanf("%d", &row_data);
    getchar();
    printf("\tColumn: ");
    scanf("%d", &column_data);
    getchar();

    r = row_data - 1;
    c = column_data - 1;
    aux = r_h[r];

    while (aux != NULL && aux->next != NULL && aux->column != column_data)
    {
        aux = aux->next;
    }

    if (r_h[r] == NULL)
    {
        printf("\tNo such node was found (invalid row)\n");
        printf("\tWanna try again? (press 'y' for yes, or 'n' for no): ");
        scanf("%c", &a);
        getchar();
        if (a == 'y')
            cyclefinder(r_h);
        else
            return;
    }
    else if (aux->column != column_data || r_h[c] == NULL)
    {
        printf("\tNo such node was found (invalid column - either the node does not exist, or it is not connected to the 'row' node)\n");
        printf("\tWanna try again? (press 'y' for yes, or 'n' for no): ");
        scanf("%c", &a);
        getchar();
        if (a == 'y')
            cyclefinder(r_h);
        else
            return;
    }
    else if (r_h[r]->parent == r_h[c] || r_h[c]->parent == r_h[r])
    {
        printf("\tInvalid nodes (their edge belongs in the bfs spanning tree)\n");
        printf("\tWanna try again? (press 'y' for yes, or 'n' for no): ");
        scanf("%c", &a);
        getchar();
        if (a == 'y')
            cyclefinder(r_h);
        else
            return;
    }
    else
    {
        typedef struct Parentlist *plist;
        struct Parentlist
        {
            node_pointer member;
            plist next;
            plist back;
        };

        plist tail_r, tail_c;
        plist head_r, head_c;
        plist aux_r, aux_c;
        plist tmp;

        // Creating a list for the 'row' node
        head_r = (plist)malloc(sizeof(struct Parentlist));
        head_r->member = r_h[r];
        head_r->back = NULL;
        head_r->next = NULL;
        tail_r = head_r;
        aux_r = head_r;
        while (aux_r != NULL && aux_r->member != NULL)
        {
            tmp = (plist)malloc(sizeof(struct Parentlist));
            tmp->member = aux_r->member->parent;
            if (tmp->member == NULL)
            {
                free(tmp);
                break;
            }
            tmp->back = aux_r;
            tmp->next = NULL;
            aux_r->next = tmp;
            tail_r = tmp;
            aux_r = aux_r->next;
        }

        // Creating a list for the 'column' node
        head_c = (plist)malloc(sizeof(struct Parentlist));
        head_c->member = r_h[c];
        head_c->back = NULL;
        head_c->next = NULL;
        tail_c = head_c;
        aux_c = head_c;
        while (aux_c != NULL && aux_c->member != NULL)
        {
            tmp = (plist)malloc(sizeof(struct Parentlist));
            tmp->member = aux_c->member->parent;
            if (tmp->member == NULL)
            {
                free(tmp);
                break;
            }
            tmp->next = NULL;
            tmp->back = aux_c;
            aux_c->next = tmp;
            tail_c = tmp;
            aux_c = aux_c->next;
        }

        aux_r = tail_r;
        aux_c = tail_c;

        while (aux_r->member == aux_c->member)
        {
            aux_r = aux_r->back;
            aux_c = aux_c->back;

            tmp = aux_r->next;
            aux_r->next = NULL;
            tail_r = aux_r;
            free(tmp);

            tmp = aux_c->next;
            aux_c->next = NULL;
            tail_c = aux_c;
            free(tmp);
        }

        printf("\n\tPRINTING THE CYCLE\n\n");
        aux_r = head_r;
        aux_c = tail_c;

        while (aux_r != NULL)
        {
            printf("\t%d\n", aux_r->member->row);
            tmp = aux_r;
            if (aux_r->next == NULL)
            {
                printf("\t%d\n", aux_r->member->parent->row);
                aux_r = NULL;
                free(tmp);
                break;
            }
            else
            {
                aux_r = aux_r->next;
                aux_r->back = NULL;
                head_r = aux_r;
                free(tmp);
            }
        }

        while (aux_c != NULL)
        {
            printf("\t%d\n", aux_c->member->row);
            tmp = aux_c;
            if (aux_c->back == NULL)
            {
                aux_c = NULL;
                free(tmp);
                break;
            }
            else
            {
                aux_c = aux_c->back;
                aux_c->next = NULL;
                tail_c = aux_c;
                free(tmp);
            }
        }

        printf("\n\tWanna run again? (press 'y' for yes, or 'n' for no): ");
        scanf("%c", &a);
        getchar();
        if (a == 'y')
            cyclefinder(r_h);
        else
            return;
    }
}