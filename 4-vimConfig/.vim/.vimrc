"  [y,d,c][number][l,w,?]
"        y-copy  d-delete  c-cut
"  l-letter  w-word  ?-line
"
"  <C-w>v   <C-w>s
"
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"" map，noremap，unmap，mapclear四个 命令，并且有四个前缀i,c,n,v修饰
""
"" n-表示在普通模式下生效
"" v-表示在可视模式下生效
"" i-表示在插入模式下生效
"" c-表示在命令行模式下生效
"" nore-表示非递归，见下面的介绍
""
"" map-表示递归的映射
""
"" unmap-表示删除某个映射
"" mapclear-表示清楚映射
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 显示相关
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"set shortmess=atI   " 启动的时候不显示那个援助乌干达儿童的提示 
"winpos 5 5          " 设定窗口位置  
"set lines=40 columns=155    " 设定窗口大小  
set go=             " 不要图形按钮  
"color asmanian2     " 设置背景主题  
"set guifont=Courier_New:h10:cANSI   " 设置字体  
syntax on           " 语法高亮  
autocmd InsertLeave * se nocul  " 用浅色高亮当前行  
autocmd InsertEnter * se cul    " 用浅色高亮当前行  
"set ruler           " 显示标尺  
set showcmd         " 输入的命令显示出来，看的清楚些  
"set cmdheight=1     " 命令行（在状态行下）的高度，设置为1  
"set whichwrap+=<,>,h,l   " 允许backspace和光标键跨越行边界(不建议)  
"set scrolloff=3     " 光标移动到buffer的顶部和底部时保持3行距离  
set novisualbell    " 不要闪烁(不明白)  
set statusline=%F%m%r%h%w\ [FORMAT=%{&ff}]\ [TYPE=%Y]\ [POS=%l,%v][%p%%]\ %{strftime(\"%d/%m/%y\ -\ %H:%M\")}   "状态行显示的内容  
set laststatus=1    " 启动显示状态行(1),总是显示状态行(2)  
" set foldenable      " 允许折叠  
" set foldmethod=manual   " h,.java文件，自动插入文件头 
autocmd BufNewFile *.cpp,*.[ch],*.sh,*.java exec ":call SetTitle()"
""定义函数SetTitle，自动插入文件头 
func SetTitle()
    "如果文件类型为.sh文件 
    if &filetype == 'sh'
        call setline(1,"\#########################################################################")
        call append(line("."), "\# File Name: ".expand("%"))
        call append(line(".")+1, "\# Author: Hello")
        call append(line(".")+2, "\# mail: Hello@163.com")
        call append(line(".")+3, "\# Created Time: ".strftime("%c"))
        call append(line(".")+4, "\#########################################################################")
        call append(line(".")+5, "\#!/bin/bash")
        call append(line(".")+6, "")
    else
        call setline(1, "/*************************************************************************")
        call append(line("."), "\# File Name: ".expand("%"))
        call append(line(".")+1, "\# Author: Hello")
        call append(line(".")+2, "\# mail: Hello@163.com")
        call append(line(".")+3, "\# Created Time: ".strftime("%c"))
        call append(line(".")+4, " ************************************************************************/")
        call append(line(".")+5, "")
    endif
    if &filetype == 'cpp'
        call append(line(".")+6, "#include<iostream>")
        call append(line(".")+7, "using namespace std;")
        call append(line(".")+8, "")
    endif
    if &filetype == 'c'
        call append(line(".")+6, "#include<stdio.h>")
        call append(line(".")+7, "")
    endif
    "   if &filetype == 'java'
    "       call append(line(".")+6,"public class ".expand("%"))
    "       call append(line(".")+7,"")
    "   endif
    "新建文件后，自动定位到文件末尾
    autocmd BufNewFile * normal G
endfunc
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"键盘命令
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let mapleader='-'
map <leader>ve :vsplit $MYVIMRC<CR>
map <leader>vs :source $MYVIMRC<CR>
nmap <leader>w :w!<cr>
nmap <leader>f :find<cr>
" 映射全选+复制 ctrl+a
map <C-A> ggVGY
map! <C-A> <Esc>ggVGY
map <F12> gg=G
" 选中状态下 Ctrl+c 复制
vmap <C-c> "+y
"去空行  
"nnoremap <F2> :g/^\s*$/d<CR> 
"比较文件  
nnoremap <C-F2> :vert diffsplit
"新建标签  
"map <M-F2> :tabnew<CR>  
"列出当前目录文件  
"map <F3> :tabnew .<CR>  
"打开树状文件目录  
map <C-F3> \be
"C，C++ 按F5编译运行
map <F5> :call CompileRunGcc()<CR>
func! CompileRunGcc()
    exec "w"
    if &filetype == 'c'
        exec "!g++ % -o %<"
        exec "! ./%<"
    elseif &filetype == 'cpp'
        exec "!g++ % -o %<"
        exec "! ./%<"
    elseif &filetype == 'java'
        exec "!javac %"
        exec "!java %<"
    elseif &filetype == 'sh'
        :!./%
    elseif &filetype == 'py'
        exec "!python %"
        exec "!python %<"
    endif
endfunc
"C,C++的调试
map <F8> :call Rungdb()<CR>
func! Rungdb()
    exec "w"
    exec "!g++ % -g -o %<"
    exec "!gdb ./%<"
endfunc


""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""实用设置
set autoread         " 设置当文件被改动时自动载入
set background=dark
"set background=light
"colorscheme  molokai
"colorscheme solarized
colorscheme  gruvbox

"set t_Co=256 
"quickfix模式
set completeopt=preview,menu      "代码补全 
filetype plugin on                "允许插件
set clipboard+=unnamed            "共享剪贴板  
set nobackup                      "从不备份  
set t_Co=256 
autocmd FileType c,cpp map <buffer> <leader><space> :w<cr>:make<cr>

set completeopt=preview,menu      "代码补全 
filetype plugin on                "允许插件
set clipboard+=unnamed            "共享剪贴板  
set nobackup                      "从不备份  
set autowrite               "自动保存
set ruler                   " 打开状态栏标尺
set cursorline              " 突出显示当前行
set magic                   " 设置魔术
set guioptions-=T           " 隐藏工具栏
set guioptions-=m           " 隐藏菜单栏
set foldcolumn=0
set foldlevel=3
set nocompatible            " 不要使用vi的键盘模式，而是vim自己的
set syntax=on               " 语法高亮
set confirm                 " 在处理未保存或只读文件的时候，弹出确认
set autoindent              " 自动缩进
set cindent
set tabstop=4               " Tab键的宽度
set softtabstop=4           " 统一缩进为4
set shiftwidth=4

set noexpandtab             " 不要用空格代替制表符
set smarttab                " 在行和段开始处使用制表符
set number                  " 显示行号
set history=3000            " 历史记录数
set nobackup                " 禁止生成临时文件
set noswapfile
set ignorecase              " 搜索忽略大小写
set hlsearch                " 搜索逐字符高亮
set incsearch
set gdefault                " 行内替换
set enc=utf-8               " 编码设置

set langmenu=zh_CN.UTF-8    "语言设置
set helplang=cn

set laststatus=2            " 总是显示状态行
set cmdheight=1             " 命令行（在状态行下）的高度，默认为1，这里是2
filetype on                 " 侦测文件类型
filetype plugin on          " 载入文件类型插件
filetype indent on          " 为特定文件类型载入相关缩进文件
set viminfo+=!              " 保存全局变量
set iskeyword+=_,$,@,%,#,-  " 带有如下符号的单词不要被换行分割
set linespace=0             " 字符间插入的像素行数目
set wildmenu                " 增强模式中的命令行自动完成操作
set backspace=2             " 使回格键（backspace）正常处理indent, eol, start等
set whichwrap+=<,>,h,l      " 允许backspace和光标键跨越行边界
set mouse=a                 " 可以在buffer的任何地方使用鼠标（类似office中在工作区双击鼠标定位）
set selection=exclusive
set selectmode=mouse,key    " 通过使用: commands命令，告诉我们文件的哪一行被改变过
set report=0                " 在被分割的窗口间显示空白，便于阅读
set fillchars=vert:\ ,stl:\ ,stlnc:\   " 高亮显示匹配的括号
set showmatch               " 匹配括号高亮的时间（单位是十分之一秒）
set matchtime=1             " 光标移动到buffer的顶部和底部时保持3行距离
set scrolloff=3             " 为C程序提供自动缩进
set smartindent
filetype plugin indent on
"打开文件类型检测, 加了这句才可以用智能补全
set completeopt=longest,menu



"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""Vundle
nnoremap <leader>pi  :PluginInstall<CR>
nnoremap <leader>pi! :PluginInstall!<CR>
nnoremap <leader>pu  :PluginUpdate<CR>
nnoremap <leader>ps  :PluginSearch<CR>
nnoremap <leader>ps! :PluginSearch!<CR>
nnoremap <leader>pc  :PluginClean<CR>
nnoremap <leader>pc! :PluginClean!<CR>
nnoremap <leader>pl  :PluginList<CR>
set nocompatible              " 去除VI一致性,必须
" filetype off                  " 必须
" " 设置包括vundle和初始化相关的runtime path
" " 另一种选择, 指定一个vundle安装插件的路径call
" vundle#begin('~/some/path/here')
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" " 让vundle管理插件版本,必须
Plugin 'VundleVim/Vundle.vim'
" 1-以下范例用来支持不同格式的插件安装.
Plugin 'ryanoasis/vim-devicons'
"Plugin 'ryanoasis/nerd-fonts'
Plugin 'vim-scripts/taglist.vim'
Plugin 'majutsushi/tagbar'

Plugin 'vim-airline/vim-airline'
Plugin 'vim-airline/vim-airline-themes'

Plugin 'scrooloose/nerdtree'
Plugin 'jistr/vim-nerdtree-tabs'
Plugin 'Xuyuanp/nerdtree-git-plugin'

Plugin 'tpope/vim-fugitive'
Plugin 'airblade/vim-gitgutter'
Plugin 'junegunn/gv.vim'
Plugin 'gregsexton/gitv'

Plugin 'flazz/vim-colorschemes'

Plugin 'tmux/tmux'
Plugin 'ycm-core/YouCompleteMe'
Plugin 'Yggdroot/indentLine'
Plugin 'mbbill/undotree'
Plugin 'bronson/vim-trailing-whitespace'
Plugin 'kien/ctrlp.vim'
Plugin 'jiangmiao/auto-pairs'
Plugin 'scrooloose/nerdcommenter'
Plugin 'tpope/vim-repeat'


" 2-来自 http://vim-scripts.org/vim/scripts.html 的插件 Plugin '插件名称'实际上是 Plugin 'vim-scripts/插件仓库名' 只是此处的用户名可以省略
Plugin 'L9'

" " 3-由Git支持但不再github上的插件仓库 Plugin 'git clone 后面的地址'
Plugin 'git://git.wincent.com/command-t.git'

" 4-本地的Git仓库(例如自己的插件) Plugin 'file:///+本地插件仓库绝对路径'
" Plugin 'file:///home/gmarik/path/to/plugin'

" 插件在仓库的子目录中.正确指定路径用以设置runtimepath.以下范例插件在sparkup/vim目录下
Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" 安装L9，如果已经安装过这个插件，可利用以下格式避免命名冲突
" Plugin 'ascenator/L9', {'name': 'newL9'}

" 你的所有插件需要在下面这行之前
call vundle#end()            " 必须
filetype plugin indent on    " 必须 加载vim自带和插件相应的语法和文件类型相关脚本

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""Tmux

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""devicons 
set encoding=UTF-8
let g:webdevicons_enable_nerdtree = 1
let g:webdevicons_conceal_nerdtree_brackets = 1

let g:webdevicons_enable_airline_tabline = 1
let g:webdevicons_enable_airline_statusline = 1


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""Tagbar
nmap <F2> :TagbarToggle<CR>
let g:tagbar_autopreview = 1
let g:tagbar_sort = 0
let g:tagbar_width=30
let g:tagbar_ctags_bin='ctags'
let g:tagbar_left = 1


""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""gList
map <F3> :TlistToggle<CR>
let Tlist_WinHeight = 20
let Tlist_Auto_Open = 0
let Tlist_Auto_Update = 0
set tags+=tags;   "设置tags的存放目录
let Tlist_Show_One_File=1   " 只允许taglist显示一个文件的信息
let Tlist_Exit_onlyWindow=1   " 当显示taglist信息的窗口是最后一个时，退出vim
let Tlist_Process_File_Always=1 " 时时更新taglist
let Tlist_Auto_Highlight_Tag = 0
let Tlist_Compact_Format = 1
let Tlist_Ctags_Cmd = '/usr/bin/ctags'
let Tlist_WinWidth=30

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""nerdtre
map <F4> :NERDTreeToggle<CR>
"autocmd vimenter * NERDTree
let g:NERDTreeWinSize = 25
let g:NERDTreeWinPos = "right"
let g:NERDTreeShowLineNumbers = 0
let NERDTreeIgnore=['\.pyc','\~$','\.swp']
let NERDTreeShowHidden=1
let NERDTreeShowBookmarks=1

autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 1 && isdirectory(argv()[0]) && !exists("s:std_in") | exe 'NERDTree' argv()[0] | wincmd p | ene | exe 'cd '.argv()[0] | endif
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif
autocmd BufEnter * if bufname('#') =~# "^NERD_tree_" && winnr('$') > 1 | b# | endif


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""vim-nerdtree-tab
let g:nerdtree_tabs_no_startup_for_diff =1
let g:nerdtree_tabs_smart_startup_focus =1
let g:nerdtree_tabs_autoclose =1
let g:nerdtree_tabs_synchronize_view =1

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""nerdtree-git
let g:NERDTreeShowIgnoredStatus = 1
let g:NERDTreeIndicatorMapCustom = {
			\ "Modified"  : "✹",
			\ "Staged"    : "✚",
			\ "Untracked" : "✭",
			\ "Renamed"   : "➜",
			\ "Unmerged"  : "═",
			\ "Deleted"   : "✖",
			\ "Dirty"     : "✗",
			\ "Clean"     : "✔︎",
			\ "Unknown"   : "?",
			\}


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""gitgutter
let g:gitgutter_enabled = 1
let g:gitgutter_highlight_lines = 1
let g:gitgutter_highlight_linenrs = 1
let g:gitgutter_async = 0
let g:gitgutter_preview_win_floating = 1
set updatetime=100
let g:gitgutter_max_signs = 500  " default value"
" let g:gitgutter_diff_relative_to = 'working_tree'
" jump to next hunk (change): ]c
" jump to previous hunk (change): [c

function! CleanUp(...)
	if a:0  " opfunc
		let [first, last] = [line("'["), line("']")]
	else
		let [first, last] = [line("'<"), line("'>")]
	endif
	for lnum in range(first, last)
		let line = getline(lnum)
		let line = substitute(line, '\s\+$', '', '')
		call setline(lnum, line)
	endfor
endfunction
nmap <silent> <Leader>x :set opfunc=CleanUp<CR>g@

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""airline
let g:airline#extensions#tabline#enabled = 1
let g:airline#extensions#tabline#formatter = 'unique_tail'
let g:airline_powerline_fonts=1
let g:airline_theme='molokai'
"let g:airline_solarized_bg='dark'
"let g:airline_theme='tomorrow'
"let g:airline_theme='simple'

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""YouCompleteMe
let g:ycm_use_clangd = 0
let g:ycm_global_ycm_extra_conf='~/.vim/bundle/YouCompleteMe/.ycm_extra_conf.py'
set runtimepath+=~/.vim/bundle/YouCompleteMe
let g:ycm_collect_identifiers_from_tags_files = 1           "开启 YCM 基于标签引擎
let g:ycm_collect_identifiers_from_comments_and_strings = 1 "注释与字符串中的内容也用于补全
let g:syntastic_ignore_files=[".*\.py$"]
let g:ycm_seed_identifiers_with_syntax = 1                  "语法关键字补全
let g:ycm_complete_in_comments = 1
let g:ycm_confirm_extra_conf = 0
let g:ycm_complete_in_comments = 1                          "在注释输入中也能补全
let g:ycm_complete_in_strings = 1                           "在字符串输入中也能补全
let g:ycm_collect_identifiers_from_comments_and_strings = 1 "注释和字符串中的文字也会被收入补全
let g:ycm_show_diagnostics_ui = 0                           "禁用语法检查
inoremap <expr> <CR> pumvisible() ? "\<C-y>" :  "\<CR>"        "回车即选中当前项
nnoremap <c-j> :YcmCompleter GoToDefinitionElseDeclaration<CR>   "跳转到定义处
let g:ycm_min_num_of_chars_for_completion=2                         "从第2个键入字符就开始罗列匹配项
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
