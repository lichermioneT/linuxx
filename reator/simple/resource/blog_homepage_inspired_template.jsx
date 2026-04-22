export default function BlogHomepageInspiredTemplate() {
  const navItems = [
    "首页",
    "C语言",
    "C++",
    "Linux",
    "数据结构",
    "设计模式",
    "Qt",
    "更多",
  ];

  const posts = [
    {
      id: 1,
      category: "C++",
      title: "auto 和 decltype 的改进",
      excerpt:
        "这里放文章摘要。你后面只需要改标题、分类、时间和摘要，就能继续扩展整站内容。",
      date: "2026-04-08",
      update: "2026-04-14",
      tag: "C++14",
      cover:
        "https://images.unsplash.com/photo-1515879218367-8466d910aaa4?auto=format&fit=crop&w=1200&q=80",
      featured: true,
    },
    {
      id: 2,
      category: "Linux",
      title: "进程、PCB 与地址空间入门",
      excerpt:
        "你可以把这里换成自己的文章简介，也可以继续复制这张卡片，快速扩展文章列表。",
      date: "2026-04-05",
      update: "2026-04-09",
      tag: "多进程",
      cover:
        "https://images.unsplash.com/photo-1555949963-aa79dcee981c?auto=format&fit=crop&w=1200&q=80",
    },
    {
      id: 3,
      category: "数据结构",
      title: "链表、栈和队列的学习路线",
      excerpt:
        "保留博客风格的卡片式展示，同时把结构做成模板化，方便你后面自己加内容。",
      date: "2026-04-01",
      update: "2026-04-02",
      tag: "基础",
      cover:
        "https://images.unsplash.com/photo-1516116216624-53e697fedbea?auto=format&fit=crop&w=1200&q=80",
    },
    {
      id: 4,
      category: "设计模式",
      title: "单例、工厂、观察者模式速查",
      excerpt:
        "这种布局比较适合技术博客首页：左侧文章流，右侧信息和导航块。",
      date: "2026-03-29",
      update: "2026-03-29",
      tag: "模式",
      cover:
        "https://images.unsplash.com/photo-1451187580459-43490279c0fa?auto=format&fit=crop&w=1200&q=80",
    },
  ];

  const tags = [
    "C++",
    "C++11",
    "C++14",
    "Linux",
    "多线程",
    "多进程",
    "网络编程",
    "Qt",
    "数据结构",
    "设计模式",
    "算法",
    "gdb",
  ];

  const archives = [
    { month: "四月 2026", count: 4 },
    { month: "三月 2026", count: 6 },
    { month: "二月 2026", count: 3 },
    { month: "一月 2026", count: 5 },
    { month: "十二月 2025", count: 8 },
  ];

  const links = [
    { name: "C/C++ 学习路线", href: "#" },
    { name: "Linux 笔记", href: "#" },
    { name: "项目合集", href: "#" },
    { name: "最新课程", href: "#" },
  ];

  const categories = [
    { name: "C语言", count: 42 },
    { name: "C++", count: 36 },
    { name: "Linux", count: 18 },
    { name: "多线程", count: 15 },
    { name: "套接字通信", count: 13 },
    { name: "数据结构", count: 20 },
    { name: "设计模式", count: 25 },
    { name: "Qt", count: 12 },
  ];

  return (
    <div className="min-h-screen bg-slate-100 text-slate-800">
      <header className="sticky top-0 z-50 border-b border-white/20 bg-slate-900/80 backdrop-blur">
        <div className="mx-auto flex max-w-7xl items-center justify-between px-4 py-3 text-white">
          <div>
            <div className="text-lg font-bold tracking-wide">你的技术博客</div>
            <div className="text-xs text-slate-300">分享编程 · 记录成长 · 沉淀知识</div>
          </div>
          <nav className="hidden gap-5 text-sm md:flex">
            {navItems.map((item) => (
              <a
                key={item}
                href="#"
                className="transition hover:text-cyan-300"
              >
                {item}
              </a>
            ))}
          </nav>
        </div>
      </header>

      <section className="relative overflow-hidden bg-slate-900 text-white">
        <img
          src="https://images.unsplash.com/photo-1516321318423-f06f85e504b3?auto=format&fit=crop&w=1800&q=80"
          alt="hero"
          className="absolute inset-0 h-full w-full object-cover opacity-30"
        />
        <div className="absolute inset-0 bg-gradient-to-r from-slate-950 via-slate-900/80 to-slate-900/50" />
        <div className="relative mx-auto grid max-w-7xl gap-8 px-4 py-16 md:grid-cols-[1.5fr_0.9fr] md:py-24">
          <div className="space-y-5">
            <span className="inline-flex rounded-full border border-cyan-400/30 bg-cyan-400/10 px-3 py-1 text-sm text-cyan-200">
              技术博客首页模板
            </span>
            <h1 className="max-w-3xl text-4xl font-black leading-tight md:text-6xl">
              做一个像技术站首页那样清晰、干净、适合长期更新的博客前端
            </h1>
            <p className="max-w-2xl text-sm leading-7 text-slate-200 md:text-base">
              这个模板保留了技术博客常见的视觉结构：顶部导航、封面区、文章流、标签区、归档区、推荐区、侧边栏个人信息。
              你后面只需要替换数组里的内容，就能继续扩展。
            </p>
            <div className="flex flex-wrap gap-3">
              <a
                href="#content"
                className="rounded-2xl bg-cyan-400 px-5 py-3 text-sm font-semibold text-slate-900 shadow-lg transition hover:scale-[1.02]"
              >
                查看文章
              </a>
              <a
                href="#"
                className="rounded-2xl border border-white/20 px-5 py-3 text-sm font-semibold transition hover:bg-white/10"
              >
                修改我的内容
              </a>
            </div>
          </div>

          <div className="grid gap-4">
            <div className="rounded-3xl border border-white/10 bg-white/10 p-6 shadow-2xl backdrop-blur">
              <div className="mb-3 flex items-center gap-4">
                <img
                  src="https://images.unsplash.com/photo-1500648767791-00dcc994a43e?auto=format&fit=crop&w=300&q=80"
                  alt="avatar"
                  className="h-16 w-16 rounded-2xl object-cover"
                />
                <div>
                  <div className="text-xl font-bold">你的名字</div>
                  <div className="text-sm text-slate-300">专注 C/C++ · Linux · 系统编程</div>
                </div>
              </div>
              <p className="text-sm leading-7 text-slate-200">
                这里可以写个人介绍、博客定位、更新说明，或者放一句欢迎语。
              </p>
            </div>

            <div className="grid grid-cols-3 gap-3">
              <div className="rounded-3xl border border-white/10 bg-white/10 p-4 text-center backdrop-blur">
                <div className="text-2xl font-black">68</div>
                <div className="text-xs text-slate-300">文章</div>
              </div>
              <div className="rounded-3xl border border-white/10 bg-white/10 p-4 text-center backdrop-blur">
                <div className="text-2xl font-black">12</div>
                <div className="text-xs text-slate-300">分类</div>
              </div>
              <div className="rounded-3xl border border-white/10 bg-white/10 p-4 text-center backdrop-blur">
                <div className="text-2xl font-black">99+</div>
                <div className="text-xs text-slate-300">标签</div>
              </div>
            </div>
          </div>
        </div>
      </section>

      <main
        id="content"
        className="mx-auto grid max-w-7xl gap-6 px-4 py-8 lg:grid-cols-[1.65fr_0.8fr]"
      >
        <section className="space-y-6">
          <div className="rounded-[28px] bg-white p-5 shadow-sm ring-1 ring-slate-200">
            <div className="mb-4 flex items-center justify-between">
              <h2 className="text-xl font-bold">文章</h2>
              <button className="rounded-xl bg-slate-100 px-3 py-2 text-sm font-medium text-slate-600">
                归档
              </button>
            </div>

            <div className="grid gap-5">
              {posts.map((post) => (
                <article
                  key={post.id}
                  className="overflow-hidden rounded-[28px] border border-slate-200 bg-slate-50 transition hover:-translate-y-0.5 hover:shadow-md"
                >
                  <div className="grid md:grid-cols-[280px_1fr]">
                    <div className="relative min-h-[220px]">
                      <img
                        src={post.cover}
                        alt={post.title}
                        className="absolute inset-0 h-full w-full object-cover"
                      />
                      <div className="absolute left-4 top-4 rounded-full bg-white/90 px-3 py-1 text-xs font-semibold text-slate-700">
                        {post.category}
                      </div>
                      {post.featured ? (
                        <div className="absolute right-4 top-4 rounded-full bg-amber-400 px-3 py-1 text-xs font-bold text-slate-900">
                          最新
                        </div>
                      ) : null}
                    </div>

                    <div className="flex flex-col justify-between p-5 md:p-6">
                      <div>
                        <h3 className="mb-3 text-2xl font-bold leading-tight text-slate-900">
                          {post.title}
                        </h3>
                        <p className="line-clamp-3 text-sm leading-7 text-slate-600">
                          {post.excerpt}
                        </p>
                      </div>

                      <div className="mt-6 flex flex-wrap items-center gap-3 text-xs text-slate-500">
                        <span className="rounded-full bg-slate-200 px-3 py-1">{post.tag}</span>
                        <span>发表于 {post.date}</span>
                        <span>更新于 {post.update}</span>
                      </div>
                    </div>
                  </div>
                </article>
              ))}
            </div>
          </div>
        </section>

        <aside className="space-y-6">
          <div className="rounded-[28px] bg-white p-5 shadow-sm ring-1 ring-slate-200">
            <h3 className="mb-4 text-lg font-bold">兴趣点</h3>
            <div className="flex flex-wrap gap-2">
              {tags.map((tag) => (
                <a
                  key={tag}
                  href="#"
                  className="rounded-full bg-slate-100 px-3 py-2 text-xs font-medium text-slate-600 transition hover:bg-slate-900 hover:text-white"
                >
                  {tag}
                </a>
              ))}
            </div>
          </div>

          <div className="rounded-[28px] bg-white p-5 shadow-sm ring-1 ring-slate-200">
            <h3 className="mb-4 text-lg font-bold">分类</h3>
            <div className="space-y-3">
              {categories.map((item) => (
                <div
                  key={item.name}
                  className="flex items-center justify-between rounded-2xl bg-slate-50 px-4 py-3"
                >
                  <span className="text-sm font-medium text-slate-700">{item.name}</span>
                  <span className="rounded-full bg-slate-200 px-2.5 py-1 text-xs text-slate-600">
                    {item.count}
                  </span>
                </div>
              ))}
            </div>
          </div>

          <div className="rounded-[28px] bg-white p-5 shadow-sm ring-1 ring-slate-200">
            <h3 className="mb-4 text-lg font-bold">归档</h3>
            <div className="space-y-3">
              {archives.map((item) => (
                <div
                  key={item.month}
                  className="flex items-center justify-between border-b border-slate-100 pb-3 text-sm last:border-0 last:pb-0"
                >
                  <span>{item.month}</span>
                  <span className="text-slate-500">{item.count} 篇</span>
                </div>
              ))}
            </div>
          </div>

          <div className="rounded-[28px] bg-white p-5 shadow-sm ring-1 ring-slate-200">
            <h3 className="mb-4 text-lg font-bold">随便逛逛</h3>
            <div className="space-y-3">
              {links.map((item) => (
                <a
                  key={item.name}
                  href={item.href}
                  className="block rounded-2xl bg-gradient-to-r from-slate-800 to-slate-700 px-4 py-3 text-sm font-medium text-white transition hover:opacity-90"
                >
                  {item.name}
                </a>
              ))}
            </div>
          </div>

          <div className="rounded-[28px] bg-white p-5 shadow-sm ring-1 ring-slate-200">
            <h3 className="mb-3 text-lg font-bold">使用说明</h3>
            <ul className="space-y-2 text-sm leading-7 text-slate-600">
              <li>1. 直接改顶部数组里的文章、标签、分类、归档数据。</li>
              <li>2. 你有自己的封面图时，把图片链接替换掉即可。</li>
              <li>3. 后端接入后，把静态数组换成接口返回数据。</li>
              <li>4. 这是首页模板，文章详情页后面可以再单独补。</li>
            </ul>
          </div>
        </aside>
      </main>
    </div>
  );
}
