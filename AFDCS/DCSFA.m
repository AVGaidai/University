%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Simulation model of distributed computing system functioning.
%% lam -- failure rate;
%% mu -- recovery rate;
%% N -- number of elementary machines;
%% i -- number of failure elementary machines (time = 0);
%% T -- operating time;
%% dt -- time step.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function DCSFA(lam, mu, N, i, T, dt)
    FailMachine = i;
    result = 0:dt:T;
    M2 = result;
    cnt = 1;
    for k = 0:dt:T
        r1 = rand();
        q = 1.00 - exp(-1.00 * lam * (N - FailMachine) * dt);
        if (r1 < q)
            FailMachine = FailMachine + 1;
        end

        r2 = rand();
        q = 1.00 - exp(-1.00 * mu * FailMachine * dt);
        if (r2 < q && FailMachine < N && FailMachine > 0)
            FailMachine = FailMachine - 1;
        end
        result(cnt) = FailMachine;
        M2(cnt) = FailMachine ^ 2;
        cnt = cnt + 1;
    end

    M = result;
    for j = 1:999
        FailMachine = i;
        cnt = 1;
        for k = 0:dt:T
            r1 = rand();
            q = 1.00 - exp(-1.00 * lam * (N - FailMachine) * dt);
            if (r1 < q)
                FailMachine = FailMachine + 1;
            end

            r2 = rand();
            q = 1.00 - exp(-1.00 * mu * FailMachine * dt);
            if (r2 < q && FailMachine < N && FailMachine > 0)
                FailMachine = FailMachine - 1;
            end
            M(cnt) = M(cnt) + FailMachine;
            M2(cnt) = M2(cnt) + FailMachine ^ 2;
            cnt = cnt + 1;
        end
    end

    cnt = 1;
    for k = 0:dt:T
        M(cnt) = M(cnt) / 1000;
        M2(cnt) = M2(cnt) / 1000;
        cnt = cnt + 1;
    end

    D = M2 - M .^ 2;
    F = M + sqrt(D);
    t = 0:dt:T;
    plot(t, M, '--', 'Color', [0 0 0], 'LineWidth', 2);
    hold on;
    plot(t, F, 'Color', [0 0 0], 'LineWidth', 2);
    hold on;
    grid on;
    M = (lam * N) / (lam + mu) + (i * mu - (N - i) * lam) / (lam + mu) * exp(-(lam + mu) .* t);
    plot(t, M, ':', 'Color', [0 0 0], 'LineWidth', 2);
    title(['N=' num2str(N) ', \lambda= ' num2str(lam) ', \mu=' num2str(mu) ', Experments=1000']);
    xlabel('time, hours');
    ylabel('failure');
    legend('show', 'M(t)', 'M(t) + \sigma', 'M(t), analytical');
    legend('Location','southeast');
    hold off;
end
