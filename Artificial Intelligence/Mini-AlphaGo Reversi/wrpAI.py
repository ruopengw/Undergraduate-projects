import random
import math
import datetime
from copy import deepcopy

class AIPlayer:
    """
    AI 玩家
    """

    def __init__(self, color):
        """
        玩家初始化
        :param color: 下棋方，'X' - 黑棋，'O' - 白棋
        """
        self.color = color
    
    def default_policy(self, moboard, player, weight): 
        # 模拟随机下棋结果:胜利返回 0(win)，失败返回 1(lose) 
        action_list = list(moboard.get_legal_actions(player))
        if player=='X':
            order = ['X','O']
        else:
            order = ['O','X']
        count = 0
        moplayer  = player
        
        while True:
            b_list = list(moboard.get_legal_actions('X'))
            w_list = list(moboard.get_legal_actions('O'))
            is_over = len(b_list) == 0 and len(w_list) == 0  # 返回值 True/False
            if is_over == True: #游戏结束
                break
            #print(count)
            if len(action_list)!=0:
                action = action_list[0]
                for node in action_list:
                    if weight[node] > weight[action]:
                        action = node  #根据权重自动行棋
                moboard._move(action, moplayer)
            #moboard.display(None, None)    
            count += 1 #进入下一回合
            moplayer = order[count%2]
            action_list = list(moboard.get_legal_actions(moplayer)) #新的可选下棋位置

        #print("mogame ended!")
        winner, diff = moboard.get_winner()
        if winner == 0:
            win_player = 'X'
        elif winner == 1:
            win_player = 'O'
        else:
            win_player = None
            
        if win_player == self.color:
            reward = 1
        elif diff == 0:
            reward = 0.5
        else:
            reward = 0
            
        return reward
    
    
    def tree_policy(self, moboard, player, route, dic):
        # 向下搜索的过程直到叶节点，更新棋盘、路径
        child = list(moboard.get_legal_actions(player))
        if player=='X':
            order = ['X','O']
        else:
            order = ['O','X']
        depth = 0
        moplayer = player
        
        while len(child)!=0 and depth<8: #当不是叶节点
            expd = self.expand(moboard, moplayer, route, dic)
            if expd != None:
                return expd
            else:
                action = self.best_child(route, child, dic, moplayer)
                route += action  #添加路径
                moboard._move(action, moplayer) #更新棋盘
                    
                depth += 1 #进入下一回合
                moplayer = order[depth%2]
                child = list(moboard.get_legal_actions(moplayer)) #新的可选下棋位置
        #print("depth"+str(depth))
        return route #遍历达到层数上限
    
        
    def expand(self, moboard, moplayer, route, dic):
        #扩展，选择一个未被尝试过的子节点
        available = list(moboard.get_legal_actions(moplayer))
        for move in available:
            new_route = route + move   
            if new_route not in dic: #节点未被访问
                dic[new_route] = [0,0] #给字典中添加新节点
                moboard._move(move, moplayer)
                return new_route
        return None #fully expanded! 所有子节点都已访问，无法扩展
    
    
    def ucb(self,nu,qv,nv,moplayer): #checked!
        c = 2 #UCB算式的常数项
        if moplayer == self.color:
            result = 1.0*qv/nv + c * math.sqrt(2.0*math.log(nu)/nv)
        else:
            result = 1 - 1.0*qv/nv + c * math.sqrt(2.0*math.log(nu)/nv)
        return result

    def best_child(self, route, child, dic, moplayer): #checked!
        # 计算给定结点的UCB最大的action, child是存有子节点的列表，dic是所有节点的信息字典
        nv = [0]*len(child) #子节点v总访问次数 N(v)
        qv = [0]*len(child) #子节点v获胜的次数 Q(v)
        n = 0
        for i in child:
            moroute = route + i
            qv[n] = dic[moroute][0]
            nv[n] = dic[moroute][1]
            n += 1
        nu = sum(nv) #父节点u被访问的总次数 N(u)=sum(nv)
        bestchild = child[0]

        maxucb = self.ucb(nu,qv[0],nv[0],moplayer)
        for i in range(len(child)):
            node = child[i]
            m = self.ucb(nu,qv[i],nv[i],moplayer)
            if m > maxucb:
                bestchild = node
                maxucb = m
            
        return bestchild
    
    def best_action(self, board, ready, dic, player, prior):
        # 计算根结点最好的action,ready是存有子节点的列表，dic是所有节点的信息字典，prior为优先度字典
        # 若某一种落子会占领四个顶点，最优先；若能让对方在下一步占领顶点，最不优先
        nextboard = deepcopy(board)
        if player=='X':
            player_op = 'O'
        else:
            player_op = 'X'
        for move in ready:            
            nextboard._move(move, player)
            ready_op = list(nextboard.get_legal_actions(player_op))
            if ('A1' in ready_op) or ('A8' in ready_op) or ('H1' in ready_op) or ('H8' in ready_op):
                prior[move] = 0.1
        
        nv = [0]*len(ready) #子节点v总访问次数 N(v)
        qv = [0]*len(ready) #子节点v获胜的次数 Q(v)
        n = 0
        for i in ready:
            qv[n] = dic[i][0]
            nv[n] = dic[i][1]
            n += 1
        nu = sum(nv) #父节点u被访问的总次数 N(u)=sum(nv)
        bestaction = ready[0]
        maxucb = self.ucb(nu,qv[0],nv[0],player) * prior[bestaction]
        for i in range(len(ready)):
            node = ready[i]
            m = self.ucb(nu,qv[i],nv[i],player) * prior[node]
            if m > maxucb:
                bestaction = node
                maxucb = m
       
        return bestaction       
    
    
    def backup(self, route, dic, reward): #checked!
        #回溯路径上的每个节点，更新其[win,all]
        n = len(route)
        while n > 0:
            node = route[0:n]
            dic[node][0] += reward
            dic[node][1] += 1
            n -= 2
        return dic

    
    def get_move(self, board):
        """
        根据当前棋盘状态获取最佳落子位置
        :param board: 棋盘
        :return: action 最佳落子位置, e.g. 'A1'
        """
        if self.color == 'X':
            player_name = '黑棋'
            #order = ['X','O']
        else:
            player_name = '白棋'
            #order = ['O','X']
        print("请等一会，对方 {}-{} 正在思考中...".format(player_name, self.color))

        # -----------------请实现你的算法代码--------------------------------------
        
        prior = {'A1':9,'B1':1,'C1':1,'D1':1,'E1':1,'F1':1,'G1':1,'H1':9,
                 'A2':1,'B2':1,'C2':1,'D2':1,'E2':1,'F2':1,'G2':1,'H2':1,
                 'A3':1,'B3':1,'C3':1,'D3':1,'E3':1,'F3':1,'G3':1,'H3':1,
                 'A4':1,'B4':1,'C4':1,'D4':1,'E4':1,'F4':1,'G4':1,'H4':1,
                 'A5':1,'B5':1,'C5':1,'D5':1,'E5':1,'F5':1,'G5':1,'H5':1,
                 'A6':1,'B6':1,'C6':1,'D6':1,'E6':1,'F6':1,'G6':1,'H6':1,
                 'A7':1,'B7':1,'C7':1,'D7':1,'E7':1,'F7':1,'G7':1,'H7':1,
                 'A8':9,'B8':1,'C8':1,'D8':1,'E8':1,'F8':1,'G8':1,'H8':9}
        #记录实时权重信息 
        
        dic = {}
        weight = {'A1':10,'B1':-6,'C1':8,'D1':6,'E1':6,'F1':8,'G1':-6,'H1':10,
                   'A2':-6,'B2':-8,'C2':-4,'D2':-3,'E2':-3,'F2':-4,'G2':-8,'H2':-6,  
                   'A3':8,'B3':-4,'C3':7,'D3':4,'E3':4,'F3':7,'G3':-4,'H3':8,
                   'A4':6,'B4':-3,'C4':4,'D4':0,'E4':0,'F4':4,'G4':-3,'H4':6,
                   'A5':6,'B5':-3,'C5':4,'D5':0,'E5':0,'F5':4,'G5':-3,'H5':6,
                   'A6':8,'B6':-4,'C6':7,'D6':4,'E6':4,'F6':7,'G6':-4,'H6':8,
                   'A7':-6,'B7':-8,'C7':-4,'D7':-3,'E7':-3,'F7':-4,'G7':-8,'H7':-6,
                   'A8':10,'B8':-6,'C8':8,'D8':6,'E8':6,'F8':8,'G8':-6,'H8':10} #棋盘上每个位置的权重
        count = 0 #记录模拟的回合数
        player = self.color #玩家
        ready = list(board.get_legal_actions(player)) # ready记录所有可选择的落子位置
        if len(ready)==0: #没有地方可下
            return None
        action = random.choice(ready) #默认落子位置
        start_time = datetime.datetime.now()
        
        while (datetime.datetime.now() - start_time).seconds < 20:
            moboard = deepcopy(board) #模拟棋盘
            route = "" #记录路径
            route = self.tree_policy(moboard, player, route, dic)
            #print(route)
            #print("tree_policy ended!")
            reward = self.default_policy(moboard, player, weight)
            #print("default_policy ended!")
            dic = self.backup(route, dic, reward)    
            #print("backup ended!")

            #count += 1
        
        action = self.best_action(board, ready, dic, player, prior)
        """
        print("action: " + action)
        print("count: " + str(count))
        for i in ready:
            print(str(i) + ": " + str(dic[i]))
        """
        
        return action
