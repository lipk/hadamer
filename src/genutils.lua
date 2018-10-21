
function rep(range, data, emitfunc, func)
    if type(data) ~= 'table' then
        data = {data}
    end
    local from = 1
    local to = range
    local step = 1
    if type(range) == 'table' then
        assert(#range == 2 or #range == 3)
        from = range[1]
        to = range[2]
        if to < from then
            step = -1
        end
        step = range[3] or step
    end
    assert(data.n == nil)
    for n=from,to,step do
        data.n = n
        if func ~= nil then
            func()
        end
        emitfunc(data)
    end
end

function emitin(m, t, f)
    rep(m, t, emiti, f)
end

function emitn(m, t, f)
    rep(m, t, emit, f)
end