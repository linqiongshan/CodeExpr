# LICENSE

* **Author**: github.com/maxthon147532

* **本作品采用 <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">知识共享署名-非商业性使用-相同方式共享 4.0 国际许可协议</a> 进行许可**

* ![CC Claim Picture](assets/by-nc-sa 4.0.png)

# Typora

* 官方帮助 [Typora IO](http://support.typora.io/)

## 功能点

### HTML 语法支持

### 基本功能

#### 快捷方式

* 设置标题级别： <kbd>Ctrl</kbd>+<kbd>1~6</kbd>

  可以快速设置标题1到标题6

* 全局搜索：<kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>F</kbd>

#### 图片插入

1. 在 `文件`, `偏好设置`, `图片插入` 可与选择插入图片时，将图片保存到 md 文件所在目录，并使用本地相对路径指向图片。可以方便的将资源文件和 md 放在一起管理
2. 从其它位置复制图片后，可以直接 <kbd>Ctrl</kbd>+<kbd>C</kbd> 复制插入 md 文档

#### 全局搜索

* <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>F</kbd>
* 文件列表左下方，搜索

  <img src="assets/1555939530686.png" style="zoom:50%" />

### 个性化配置

#### 添加自定义 CSS 文件

1. <kbd>文件</kbd> / <kbd>偏好设置</kbd> / <kbd>打开主题文件夹</kbd>
2. 主题文件夹下，可以放 .CSS 文件。Typora 的 CSS 文件加载顺序是
   1. Typora’s basic styles.
   2. CSS for current theme.
   3. `base.user.css` under theme folder.
   4. `{current-theme}.user.css` under theme folder. If you choose `Github` as your theme, then `github.user.css` will also be loaded.
3. 定义自己的 CSS
   1. 如果希望对所有主题都生效，可以修改 `base.user.css`
   2. 如果希望只修改特定主题，可以修改 `{theme-name}.user.css`

#### 自定义 CSS 实例

##### 标题自动编号

  ```css
  /** initialize css counter */
  #write {
    counter-reset: h1
  }
  
  h1 {
      counter-reset: h2
  }
  
  h2 {
      counter-reset: h3
  }
  
  h3 {
      counter-reset: h4
  }
  
  h4 {
      counter-reset: h5
  }
  
  h5 {
      counter-reset: h6
  }
  
  /** put counter result into headings */
  #write h1:before {
      counter-increment: h1;
      content: counter(h1) ". "
  }
  
  #write h2:before {
      counter-increment: h2;
      content: counter(h1) "." counter(h2) ". "
  }
  
  #write h3:before,
  h3.md-focus.md-heading:before /** override the default style for focused headings */ {
      counter-increment: h3;
      content: counter(h1) "." counter(h2) "." counter(h3) ". "
  }
  
  #write h4:before,
  h4.md-focus.md-heading:before {
      counter-increment: h4;
      content: counter(h1) "." counter(h2) "." counter(h3) "." counter(h4) ". "
  }
  
  #write h5:before,
  h5.md-focus.md-heading:before {
      counter-increment: h5;
      content: counter(h1) "." counter(h2) "." counter(h3) "." counter(h4) "." counter(h5) ". "
  }
  
  #write h6:before,
  h6.md-focus.md-heading:before {
      counter-increment: h6;
      content: counter(h1) "." counter(h2) "." counter(h3) "." counter(h4) "." counter(h5) "." counter(h6) ". "
  }
  
  /** override the default style for focused headings */
  #write>h3.md-focus:before,
  #write>h4.md-focus:before,
  #write>h5.md-focus:before,
  #write>h6.md-focus:before,
  h3.md-focus:before,
  h4.md-focus:before,
  h5.md-focus:before,
  h6.md-focus:before {
      color: inherit;
      border: inherit;
      border-radius: inherit;
      position: inherit;
      left:initial;
      float: none;
      top:initial;
      font-size: inherit;
      padding-left: inherit;
      padding-right: inherit;
      vertical-align: inherit;
      font-weight: inherit;
      line-height: inherit;
  }
  ```

##### `[TOC]`自动编号

```css
/**************************************
 * Header Counters in TOC
 **************************************/

/* No link underlines in TOC */
.md-toc-inner {
    text-decoration: none;
}

.md-toc-content {
    counter-reset: h1toc
}

.md-toc-h1 {
    margin-left: 0;
    font-size: 1.5rem;
    counter-reset: h2toc
}

.md-toc-h2 {
    font-size: 1.1rem;
    margin-left: 2rem;
    counter-reset: h3toc
}

.md-toc-h3 {
    margin-left: 3rem;
    font-size: .9rem;
    counter-reset: h4toc
}

.md-toc-h4 {
    margin-left: 4rem;
    font-size: .85rem;
    counter-reset: h5toc
}

.md-toc-h5 {
    margin-left: 5rem;
    font-size: .8rem;
    counter-reset: h6toc
}

.md-toc-h6 {
    margin-left: 6rem;
    font-size: .75rem;
}

.md-toc-h1:before {
    color: black;
    counter-increment: h1toc;
    content: counter(h1toc) ". "
}

.md-toc-h1 .md-toc-inner {
    margin-left: 0;
}

.md-toc-h2:before {
    color: black;
    counter-increment: h2toc;
    content: counter(h1toc) ". " counter(h2toc) ". "
}

.md-toc-h2 .md-toc-inner {
    margin-left: 0;
}

.md-toc-h3:before {
    color: black;
    counter-increment: h3toc;
    content: counter(h1toc) ". " counter(h2toc) ". " counter(h3toc) ". "
}

.md-toc-h3 .md-toc-inner {
    margin-left: 0;
}

.md-toc-h4:before {
    color: black;
    counter-increment: h4toc;
    content: counter(h1toc) ". " counter(h2toc) ". " counter(h3toc) ". " counter(h4toc) ". "
}

.md-toc-h4 .md-toc-inner {
    margin-left: 0;
}

.md-toc-h5:before {
    color: black;
    counter-increment: h5toc;
    content: counter(h1toc) ". " counter(h2toc) ". " counter(h3toc) ". " counter(h4toc) ". " counter(h5toc) ". "
}

.md-toc-h5 .md-toc-inner {
    margin-left: 0;
}

.md-toc-h6:before {
    color: black;
    counter-increment: h6toc;
    content: counter(h1toc) ". " counter(h2toc) ". " counter(h3toc) ". " counter(h4toc) ". " counter(h5toc) ". " counter(h6toc) ". "
}

.md-toc-h6 .md-toc-inner {
    margin-left: 0;
}
```

##### 侧边栏大纲自动编号

```css
.sidebar-content {
    counter-reset: h1
}

.outline-h1 {
    counter-reset: h2
}

.outline-h2 {
    counter-reset: h3
}

.outline-h3 {
    counter-reset: h4
}

.outline-h4 {
    counter-reset: h5
}

.outline-h5 {
    counter-reset: h6
}

.outline-h1>.outline-item>.outline-label:before {
    counter-increment: h1;
    content: counter(h1) ". "
}

.outline-h2>.outline-item>.outline-label:before {
    counter-increment: h2;
    content: counter(h1) "." counter(h2) ". "
}

.outline-h3>.outline-item>.outline-label:before {
    counter-increment: h3;
    content: counter(h1) "." counter(h2) "." counter(h3) ". "
}

.outline-h4>.outline-item>.outline-label:before {
    counter-increment: h4;
    content: counter(h1) "." counter(h2) "." counter(h3) "." counter(h4) ". "
}

.outline-h5>.outline-item>.outline-label:before {
    counter-increment: h5;
    content: counter(h1) "." counter(h2) "." counter(h3) "." counter(h4) "." counter(h5) ". "
}

.outline-h6>.outline-item>.outline-label:before {
    counter-increment: h6;
    content: counter(h1) "." counter(h2) "." counter(h3) "." counter(h4) "." counter(h5) "." counter(h6) ". "
}
```



##### 任务列表：已完成，待完成状态样式

```css
.task-list-done {
    /* styles for completed tasks */
    text-decoration: line-through;
}
.task-list-not-done {
    /* styles for incomplete tasks */
}
```

