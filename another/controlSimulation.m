function drag_vector_bar
    % 交互式向量变换演示
    % 窗口1: 拖动输入向量
    % 窗口2: 结果向量的 x、y 分量柱状图

    % ----------- 参数 ------------
    Rcircle = 1;           % 输入圆半径
    theta = -pi/4;         % 顺时针45度
    R = [cos(theta) -sin(theta);
         sin(theta)  cos(theta)];
    S1 = [0 1; 1 0]; %交换矩阵
    M = S1*R;

    % ----------- 窗口1：输入向量 ------------
    fig1 = figure('Name','输入向量','NumberTitle','off');
    axis equal; hold on; grid on;
    xlim([-Rcircle Rcircle]); ylim([-Rcircle Rcircle]);
    viscircles([0,0], Rcircle,'Color',[0.7 0.7 0.7]); % 圆边界
    plot(0,0,'k+'); % 原点
    hInput = quiver(0,0,1,0,0,'r','LineWidth',2,'MaxHeadSize',0.5);
    title('在圆内拖动鼠标，输入向量');

    % ----------- 窗口2：输出柱状图 ------------
    fig2 = figure('Name','结果向量分量','NumberTitle','off');
    hBar = bar([0,0],'FaceColor',[0.2 0.5 0.8]);
    ylim([-Rcircle Rcircle]); 
    set(gca,'XTickLabel',{'y', 'x'});
    ylabel('值');
    title('结果向量 (旋转+交换) 的分量');

    % ----------- 鼠标事件回调 ------------
    set(fig1,'WindowButtonMotionFcn',@updateVector);

    function updateVector(~,~)
        % 获取鼠标坐标
        C = get(gca,'CurrentPoint');
        vx = C(1,1); vy = C(1,2);

        % 限制在圆内
        if vx^2+vy^2 > Rcircle^2
            return;
        end

        % 更新输入向量
        set(hInput,'UData',vx,'VData',vy);

        % 计算输出
        v = [vx; vy];
        result = M * v;

        % 更新柱状图
        if isvalid(hBar)
            set(hBar,'YData',result);
        end
    end
end
